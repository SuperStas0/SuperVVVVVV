#include "Supermode.h"

#include <string>

#include "Entity.h"
#include "Enums.h"
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
    static Pending pending = Pending_NONE;
    static bool restarting = false;
    static int countdown = 0;
    static bool countdown_control = false;
    static int popup_timer = 0;
    static std::string popup_text;

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
        return super_setting;
    }

    bool allow_states(void)
    {
        return allow_setting;
    }

    void toggle_super(void)
    {
        super_setting = !super_setting;
        popup(super_setting ? "Super mode on" : "Super mode off");
    }

    void toggle_allow(void)
    {
        allow_setting = !allow_setting;
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
                if (savestate::exists(make_key(mode, false)))
                {
                    return true;
                }
            }
            return false;
        }

        return savestate::exists(key());
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
            if (savestate::load(key(), true) && !super_setting)
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

        if (allow_setting && savestate::exists(key()))
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

    void popup(const char* text)
    {
        popup_text = text;
        popup_timer = POPUP_FRAMES;
    }

    bool render_popup(void)
    {
        if (popup_timer <= 0)
        {
            return false;
        }

        graphics.render_roomname(PR_FONT_INTERFACE, supertext::get(popup_text.c_str()), 196, 196, 255);
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
