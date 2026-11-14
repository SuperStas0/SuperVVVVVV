#include "Supermode.h"

#include <string>

#include <tinyxml2.h>

#include "Constants.h"
#include "Entity.h"
#include "Enums.h"
#include "FileSystemUtils.h"
#include "Font.h"
#include "Game.h"
#include "Graphics.h"
#include "Localization.h"
#include "Map.h"
#include "Music.h"
#include "Savestate.h"
#include "Script.h"
#include "Supertext.h"
#include "UtilityClass.h"
#include "VFormat.h"
#include "Vlogging.h"
#include "XMLUtils.h"

namespace supermode
{
    static const int POPUP_FRAMES = 75;
    static const int FAST_DEATH_FRAMES = 15;
    static const int DEATH_FRAMES = 30;
    static const int TRIAL_COUNTDOWN = 60;
    static const int STATE_COUNTDOWN = 60;

    enum Pending
    {
        Pending_NONE,
        Pending_RESTART,
        Pending_RESTART_INSTANT,
        Pending_LOAD
    };

    static int current_mode = -1;
    static bool trial_super = false;
    static bool super_setting = false;
    static bool allow_setting = false;
    static bool reset_confirmed = false;
    static Pending pending = Pending_NONE;
    static bool restarting = false;
    static int countdown = 0;
    static bool countdown_control = false;
    static int popup_timer = 0;
    static std::string popup_text;
    static int popup_number = 0;

    /* The settings and the agreement of the reset run confirmation stay
     * between launches. They are read when first needed. */
    static const char* const SETTINGS_FILE = "saves/super.vvv";
    static bool settings_loaded = false;

    static void store_settings(void)
    {
        tinyxml2::XMLDocument doc;
        xml::update_declaration(doc);

        tinyxml2::XMLElement* root = xml::update_element(doc, "Save");
        xml::update_comment(root, " Save file ");

        tinyxml2::XMLElement* data = xml::update_element(root, "Data");
        xml::update_tag(data, "supermode", super_setting ? 1 : 0);
        xml::update_tag(data, "allowstates", allow_setting ? 1 : 0);
        xml::update_tag(data, "resetconfirmed", reset_confirmed ? 1 : 0);

        if (!FILESYSTEM_saveTiXml2Document(SETTINGS_FILE, doc))
        {
            vlog_error("Could not write %s", SETTINGS_FILE);
        }
    }

    static void load_settings(void)
    {
        if (settings_loaded || !FILESYSTEM_isInit())
        {
            return;
        }
        settings_loaded = true;

        tinyxml2::XMLDocument doc;
        if (!FILESYSTEM_loadTiXml2Document(SETTINGS_FILE, doc))
        {
            return;
        }
        if (doc.Error())
        {
            vlog_error("Error parsing %s: %s", SETTINGS_FILE, doc.ErrorStr());
            return;
        }

        tinyxml2::XMLHandle hDoc(&doc);
        tinyxml2::XMLElement* element = hDoc
            .FirstChildElement()
            .FirstChildElement("Data")
            .FirstChildElement()
            .ToElement();

        for (; element != NULL; element = element->NextSiblingElement())
        {
            const char* name = element->Value();
            const char* text = element->GetText();
            const bool on = text != NULL && SDL_atoi(text) != 0;

            if (SDL_strcmp(name, "supermode") == 0)
            {
                super_setting = on;
            }
            else if (SDL_strcmp(name, "allowstates") == 0)
            {
                allow_setting = on;
            }
            else if (SDL_strcmp(name, "resetconfirmed") == 0)
            {
                reset_confirmed = on;
            }
        }
    }

    static bool is_ndm_mode(const int mode)
    {
        return mode >= Start_FIRST_NODEATHMODE && mode <= Start_LAST_NODEATHMODE;
    }

    static bool is_trial_mode(const int mode)
    {
        return (mode >= Start_FIRST_TIMETRIAL && mode <= Start_TIMETRIAL_FINALLEVEL)
            || (mode >= Start_FIRST_INTERMISSION1 && mode <= Start_LAST_INTERMISSION2);
    }

    static bool is_main_mode(const int mode)
    {
        return mode >= Start_MAINGAME && mode <= Start_MAINGAME_QUICKSAVE;
    }

    static int make_key(const int mode, const bool super)
    {
        /* New game, telesave and quicksave runs share one slot. */
        return (is_main_mode(mode) ? (int) Start_MAINGAME : mode) * 2 + (super ? 1 : 0);
    }

    /* The run is one that can have states. */
    static bool in_context(void)
    {
        return current_mode != -1
            && (game.intimetrial || game.inintermission || game.nodeathmode || is_main_mode(current_mode))
            && !game.translator_exploring
            && !map.custommode;
    }

    static bool in_run(void)
    {
        return in_context()
            && game.gamestate == GAMEMODE
            && !game.fadetomenu
            && !game.fadetolab;
    }

    void note_mode(enum StartMode mode)
    {
        load_settings();
        current_mode = (int) mode;
        trial_super = super_setting && is_trial_mode(current_mode);
        pending = Pending_NONE;
        countdown = 0;
        if (!restarting)
        {
            popup_timer = 0;
        }
    }

    int key(void)
    {
        return make_key(current_mode, trial_super);
    }

    bool states_allowed(void)
    {
        load_settings();
        return allow_setting && in_run() && pending == Pending_NONE;
    }

    bool counting_down(void)
    {
        return countdown > 0;
    }

    void cancel_countdown(void)
    {
        countdown = 0;
    }

    bool super_mode(void)
    {
        load_settings();
        return super_setting;
    }

    bool allow_states(void)
    {
        load_settings();
        return allow_setting;
    }

    void toggle_super(void)
    {
        load_settings();
        super_setting = !super_setting;
        store_settings();
        popup(super_setting ? "Super mode on" : "Super mode off");
    }

    void toggle_allow(void)
    {
        load_settings();
        allow_setting = !allow_setting;
        store_settings();
        popup(allow_setting ? "Savestates on" : "Savestates off");
    }

    void delete_states(void)
    {
        savestate::remove_all();
        popup("Savestates deleted");
    }

    static bool run_has_state(void)
    {
        if (current_mode == -1)
        {
            return false;
        }

        if (is_ndm_mode(current_mode))
        {
            /* Either cutscene choice counts for both. */
            for (int mode = Start_FIRST_NODEATHMODE; mode <= Start_LAST_NODEATHMODE; mode++)
            {
                if (savestate::exists_any(make_key(mode, false)))
                {
                    return true;
                }
            }
            return false;
        }

        return savestate::exists_any(key());
    }

    bool invalid_run(void)
    {
        return savestate::used() || trial_super || run_has_state();
    }

    void apply_trial_rules(void)
    {
        if (trial_super)
        {
            game.nodeathmode = true;
            game.nocutscenes = true;
        }
    }

    static int records_times[Game::numtrials];
    static int records_frames[Game::numtrials];
    static int records_trinkets[Game::numtrials];
    static int records_lives[Game::numtrials];
    static int records_rank[Game::numtrials];
    static bool records_kept = false;

    void protect_records(void)
    {
        records_kept = invalid_run();
        if (!records_kept)
        {
            return;
        }

        for (int i = 0; i < Game::numtrials; i++)
        {
            records_times[i] = game.besttimes[i];
            records_frames[i] = game.bestframes[i];
            records_trinkets[i] = game.besttrinkets[i];
            records_lives[i] = game.bestlives[i];
            records_rank[i] = game.bestrank[i];
        }
    }

    void restore_records(void)
    {
        if (!records_kept)
        {
            return;
        }
        records_kept = false;

        for (int i = 0; i < Game::numtrials; i++)
        {
            game.besttimes[i] = records_times[i];
            game.bestframes[i] = records_frames[i];
            game.besttrinkets[i] = records_trinkets[i];
            game.bestlives[i] = records_lives[i];
            game.bestrank[i] = records_rank[i];
        }
    }

    static void begin_restart(void)
    {
        graphics.fademode = FADE_START_FADEOUT;
        game.completestop = true;
        music.fadeout();
        pending = Pending_RESTART;
    }

    static void begin_countdown(void)
    {
        countdown = STATE_COUNTDOWN;
        countdown_control = game.hascontrol;
        game.hascontrol = false;
        game.completestop = true;
        music.playef(Sound_COUNTDOWN);
    }

    static void update_countdown(void)
    {
        if (countdown <= 0)
        {
            return;
        }

        countdown--;
        if (countdown > 30)
        {
            game.hascontrol = false;
            game.completestop = true;
        }
        else if (countdown == 30)
        {
            game.completestop = false;
            game.hascontrol = countdown_control;
            music.playef(Sound_GO);
        }
    }

    static void update_pending(void)
    {
        if (pending == Pending_NONE)
        {
            return;
        }
        if (pending == Pending_RESTART && graphics.fademode != FADE_FULLY_BLACK)
        {
            return;
        }

        const Pending what = pending;
        pending = Pending_NONE;

        if (what == Pending_LOAD)
        {
            if (savestate::load(key(), savestate::last(key())) && !super_setting)
            {
                begin_countdown();
            }
            return;
        }

        restarting = true;
        script.startgamemode((enum StartMode) current_mode);
        restarting = false;
        game.deathseq = -1;
        game.completestop = false;
        if (what == Pending_RESTART_INSTANT)
        {
            graphics.fademode = FADE_NONE;
        }
        if (game.intimetrial)
        {
            game.hascontrol = false;
        }
    }

    void update(void)
    {
        if (popup_timer > 0)
        {
            popup_timer--;
        }

        update_pending();
        update_countdown();

        /* The first frame of a time trial with Super mode on: shorten the countdown. */
        if (trial_super && game.intimetrial && game.timetrialcountdown > TRIAL_COUNTDOWN)
        {
            game.timetrialcountdown = TRIAL_COUNTDOWN;
        }

        /* Super mode was toggled in the options menu during a trial. */
        if (in_run()
        && is_trial_mode(current_mode)
        && super_setting != trial_super
        && graphics.fademode == FADE_NONE
        && pending == Pending_NONE)
        {
            begin_restart();
        }
    }

    /* Skips the fade and the game over screen. */
    static void leave_to_cutscene_screen(void)
    {
        game.gethardestroom();
        game.copyndmresults();
        obj.entities.clear();
        game.quittomenu();
        graphics.fademode = FADE_NONE;
        game.createmenu(Menu::startnodeathmode);
        game.currentmenuoption = current_mode == Start_NODEATHMODE_WITHCUTSCENES ? 1 : 0;
        music.play(Music_PRESENTINGVVVVVV);

        /* Keys still held from playing must not pick a menu option. The menu
         * ignores input until they are released. */
        game.jumpheld = true;
        game.mapheld = true;
    }

    bool loads_state_on_death(void)
    {
        return game.nodeathmode
            && !map.custommode
            && current_mode != -1
            && allow_setting
            && savestate::last(key()) != -1;
    }

    void death_music(void)
    {
        if (loads_state_on_death())
        {
            return;
        }

        music.fadeout();

        /* Fix a bug being able to play music on the Game Over screen */
        music.nicefade = false;
    }

    bool death_exit(void)
    {
        if (!game.nodeathmode
        || game.deathseq == -1
        || current_mode == -1
        || map.custommode)
        {
            return false;
        }

        const int wait = super_setting ? FAST_DEATH_FRAMES : DEATH_FRAMES;
        if (game.deathseq > DEATH_FRAMES - wait)
        {
            return false;
        }

        if (pending != Pending_NONE)
        {
            return true;
        }

        if (loads_state_on_death())
        {
            pending = Pending_LOAD;
            return true;
        }

        if (trial_super)
        {
            pending = Pending_RESTART_INSTANT;
            return true;
        }

        if (super_setting && is_ndm_mode(current_mode))
        {
            leave_to_cutscene_screen();
            return true;
        }

        return false;
    }

    bool reset_run_available(void)
    {
        load_settings();
        return super_setting
            && in_context()
            && is_main_mode(current_mode)
            && !game.inspecial();
    }

    /* The pages of the confirmation, shown before the first use in a save
     * without the Game Complete trophy. */
    static const int PAGE_RESET = 33;
    static const int PAGE_CONFIRM_NO = 36;
    static const int PAGE_CONFIRM_YES = 37;

    int last_pause_page(void)
    {
        return reset_run_available() ? 33 : 32;
    }

    const char* reset_run_text(void)
    {
        return supertext::get("reset run");
    }

    /* The same unlocks as the "unlock play modes" accessibility menu. */
    static void unlock_play_modes(void)
    {
        static const int unlocks[] = {
            Unlock_TIMETRIAL_SPACESTATION1,
            Unlock_TIMETRIAL_LABORATORY,
            Unlock_TIMETRIAL_TOWER,
            Unlock_TIMETRIAL_SPACESTATION2,
            Unlock_TIMETRIAL_WARPZONE,
            Unlock_TIMETRIAL_FINALLEVEL,
            Unlock_INTERMISSION_REPLAYS,
            Unlock_NODEATHMODE,
            Unlock_FLIPMODE,
            Unlock_SECRETLAB
        };

        for (size_t i = 0; i < sizeof(unlocks) / sizeof(unlocks[0]); i++)
        {
            game.unlock[unlocks[i]] = true;
            game.unlocknotify[unlocks[i]] = true;
        }
        game.unlock[Unlock_INTERMISSION1_COMPLETE] = true;
        game.unlock[Unlock_INTERMISSION2_COMPLETE] = true;
        game.stat_trinkets = 20;
    }

    static void reset_run(void)
    {
        if (!reset_run_available())
        {
            return;
        }

        if (game.last_quicksave.exists)
        {
            game.deletequick();
        }
        if (game.last_telesave.exists)
        {
            game.deletetele();
        }

        unlock_play_modes();
        game.savestatsandsettings_menu();

        music.playef(Sound_VIRIDIAN);
        game.quittomenu();
        game.createmenu(Menu::mainmenu);
        graphics.fademode = FADE_NONE;
        music.play(Music_PRESENTINGVVVVVV);
    }

    void pause_page_action(void)
    {
        load_settings();
        switch (game.menupage)
        {
        case PAGE_RESET:
            if (game.unlock[UnlockTrophy_GAME_COMPLETE] || reset_confirmed)
            {
                reset_run();
            }
            else
            {
                music.playef(Sound_VIRIDIAN);
                game.menupage = PAGE_CONFIRM_YES;
            }
            break;
        case PAGE_CONFIRM_NO:
            music.playef(Sound_VIRIDIAN);
            game.menupage = PAGE_RESET;
            break;
        case PAGE_CONFIRM_YES:
            reset_confirmed = true;
            store_settings();
            reset_run();
            break;
        }
    }

    static void selected_label(char* buffer, const size_t size, const char* text)
    {
        vformat_buf(buffer, size, loc::get_langmeta()->menu_select.c_str(), "label:str", loc::toupper(text).c_str());
    }

    void render_reset_prompt(const int selection_offset)
    {
        const bool yes_selected = game.menupage == PAGE_CONFIRM_YES;
        const bool flip = graphics.flipmode;
        const int glow = help.glow;
        const char* no_text = supertext::get("no, go back");
        const char* yes_text = supertext::get("yes, reset run");
        char buffer[SCREEN_WIDTH_CHARS + 1];

        selected_label(buffer, sizeof(buffer), supertext::get("reset run"));
        font::print(PR_CEN | PR_CJK_LOW, -1, 220, buffer, 196, 196, 255 - glow);
        font::print_wrap(PR_CEN, -1, flip ? 142 : 76, supertext::get("This should not be used by vanilla players"), 196, 196, 255 - glow, 12);

        const int no_y = flip ? 88 : 130;
        const int yes_y = flip ? 76 : 142;
        if (yes_selected)
        {
            font::print(PR_RTL_XFLIP, 80, no_y, no_text, 96, 96, 96);
            selected_label(buffer, sizeof(buffer), yes_text);
            font::print(PR_RTL_XFLIP, 80 + 32 - selection_offset, yes_y, buffer, 196, 196, 255 - glow);
        }
        else
        {
            selected_label(buffer, sizeof(buffer), no_text);
            font::print(PR_RTL_XFLIP, 80 - selection_offset, no_y, buffer, 196, 196, 255 - glow);
            font::print(PR_RTL_XFLIP, 80 + 32, yes_y, yes_text, 96, 96, 96);
        }
    }

    void popup(const char* text)
    {
        popup_text = text;
        popup_number = 0;
        popup_timer = POPUP_FRAMES;
    }

    void popup_state(const char* text, const int number)
    {
        popup_text = text;
        popup_number = number;
        popup_timer = POPUP_FRAMES;
    }

    bool render_popup(void)
    {
        if (popup_timer <= 0)
        {
            return false;
        }

        char buffer[SCREEN_WIDTH_CHARS + 1];
        const char* text = supertext::get(popup_text.c_str());
        if (popup_number > 0)
        {
            vformat_buf(buffer, sizeof(buffer), text, "n:int", popup_number);
            text = buffer;
        }

        graphics.render_roomname(PR_FONT_INTERFACE, text, 196, 196, 255);
        return true;
    }

    void render_countdown(void)
    {
        if (countdown <= 0 || graphics.fademode != FADE_NONE)
        {
            return;
        }

        const int glow = help.glow;
        if (countdown < 30)
        {
            if ((countdown / 4) % 2 == 0)
            {
                font::print(PR_4X | PR_CEN | PR_BOR, -1, 100, loc::gettext("Go!"), 220 - glow, 220 - glow, 255 - glow / 2);
            }
        }
        else if (countdown < 60)
        {
            font::print(PR_4X | PR_CEN | PR_BOR, -1, 100, "1", 220 - glow, 220 - glow, 255 - glow / 2);
        }
    }

    void print_gg(const int scale, const int y, const int r, const int g, const int b)
    {
        /* The 8x8 font has no right bearing on G and a wide one on !, so the
         * advance width is centered but the letters look shifted left. */
        const int shift = (scale * 4 + 1) / 3;
        const uint32_t size = scale == 4 ? PR_4X : PR_3X;
        font::print(size | PR_FONT_8X8 | PR_CEN, 160 + shift, y, "GG!", r, g, b);
    }

    void print_wrap_valid(const uint32_t flags, const int x, const int y, const char* text, const int r, const int g, const int b)
    {
        if (!invalid_run())
        {
            font::print_wrap(flags, x, y, text, r, g, b);
        }
    }

    void print_title(const int scale, const uint32_t flags, const int y, const char* text, const int r, const int g, const int b)
    {
        if (invalid_run())
        {
            print_gg(scale, y, r, g, b);
        }
        else
        {
            font::print(flags, -1, y, text, r, g, b);
        }
    }

    bool draw_gg(const int image, const bool flipmode)
    {
        if (image != TEXTIMAGE_LEVELCOMPLETE && image != TEXTIMAGE_GAMECOMPLETE)
        {
            return false;
        }
        if (!invalid_run())
        {
            return false;
        }

        if (image == TEXTIMAGE_LEVELCOMPLETE)
        {
            const SDL_Color color = TEXT_COLOUR("cyan");
            print_gg(3, flipmode ? 194 : 25, color.r, color.g, color.b);
        }
        else
        {
            print_gg(3, flipmode ? 194 : 25, 196, 196, 243);
        }
        return true;
    }
}
