#include "Savestate.h"

#include <SDL.h>
#include <map>
#include <string>
#include <vector>

#include "Entity.h"
#include "Enums.h"
#include "Game.h"
#include "Graphics.h"
#include "Map.h"
#include "Music.h"
#include "Script.h"

/* Whole-object snapshots of the gameplay globals. Anything that is a user
 * setting or persistent progress (not part of the run) is kept as-is on load. */

namespace savestate
{
    static const int POPUP_FRAMES = 75;
    static const int FAST_DEATH_FRAMES = 15;
    static const int TRIAL_COUNTDOWN = 60;

    struct GfxState
    {
        int rcol;
        TowerBG towerbg;
        enum FadeBars fademode;
        int fadeamount, oldfadeamount;
        int fadebars[15];
        enum FadeBars ingame_fademode;
        bool showcutscenebars;
        int cutscenebarspos, oldcutscenebarspos;
        bool trinketcolset;
        int trinketr, trinketg, trinketb;
        int linestate, linedelay;
        int backoffset;
        std::vector<textboxclass> textboxes;
    };

    struct Persist
    {
        bool muted, musicmuted;
        int mutebutton, musicmutebutton;
        bool colourblindmode, noflashingmode, over30mode, showingametimer;
        int slowdown;
        bool disablepause, disableaudiopause, inputdelay, ghostsenabled;
        bool skipfakeload, separate_interact;
        std::vector<SDL_GameControllerButton> button_map, button_flip, button_esc,
            button_restart, button_interact;
        bool unlock[Game::numunlock];
        bool unlocknotify[Game::numunlock];
        int besttimes[Game::numtrials];
        int bestframes[Game::numtrials];
        int besttrinkets[Game::numtrials];
        int bestlives[Game::numtrials];
        int bestrank[Game::numtrials];
        int stat_trinkets, bestgamedeaths, swnbestrank, swnrecord;
        std::map<std::string, int> customlevelstats;
        bool invincibility;
    };

    static void persist_capture(Persist& p)
    {
        p.muted = game.muted;
        p.musicmuted = game.musicmuted;
        p.mutebutton = game.mutebutton;
        p.musicmutebutton = game.musicmutebutton;
        p.colourblindmode = game.colourblindmode;
        p.noflashingmode = game.noflashingmode;
        p.over30mode = game.over30mode;
        p.showingametimer = game.showingametimer;
        p.slowdown = game.slowdown;
        p.disablepause = game.disablepause;
        p.disableaudiopause = game.disableaudiopause;
        p.inputdelay = game.inputdelay;
        p.ghostsenabled = game.ghostsenabled;
        p.skipfakeload = game.skipfakeload;
        p.separate_interact = game.separate_interact;
        p.button_map = game.controllerButton_map;
        p.button_flip = game.controllerButton_flip;
        p.button_esc = game.controllerButton_esc;
        p.button_restart = game.controllerButton_restart;
        p.button_interact = game.controllerButton_interact;
        SDL_memcpy(p.unlock, game.unlock, sizeof(p.unlock));
        SDL_memcpy(p.unlocknotify, game.unlocknotify, sizeof(p.unlocknotify));
        SDL_memcpy(p.besttimes, game.besttimes, sizeof(p.besttimes));
        SDL_memcpy(p.bestframes, game.bestframes, sizeof(p.bestframes));
        SDL_memcpy(p.besttrinkets, game.besttrinkets, sizeof(p.besttrinkets));
        SDL_memcpy(p.bestlives, game.bestlives, sizeof(p.bestlives));
        SDL_memcpy(p.bestrank, game.bestrank, sizeof(p.bestrank));
        p.stat_trinkets = game.stat_trinkets;
        p.bestgamedeaths = game.bestgamedeaths;
        p.swnbestrank = game.swnbestrank;
        p.swnrecord = game.swnrecord;
        p.customlevelstats = game.customlevelstats;
        p.invincibility = map.invincibility;
    }

    static void persist_apply(const Persist& p)
    {
        game.muted = p.muted;
        game.musicmuted = p.musicmuted;
        game.mutebutton = p.mutebutton;
        game.musicmutebutton = p.musicmutebutton;
        game.colourblindmode = p.colourblindmode;
        game.noflashingmode = p.noflashingmode;
        game.over30mode = p.over30mode;
        game.showingametimer = p.showingametimer;
        game.slowdown = p.slowdown;
        game.disablepause = p.disablepause;
        game.disableaudiopause = p.disableaudiopause;
        game.inputdelay = p.inputdelay;
        game.ghostsenabled = p.ghostsenabled;
        game.skipfakeload = p.skipfakeload;
        game.separate_interact = p.separate_interact;
        game.controllerButton_map = p.button_map;
        game.controllerButton_flip = p.button_flip;
        game.controllerButton_esc = p.button_esc;
        game.controllerButton_restart = p.button_restart;
        game.controllerButton_interact = p.button_interact;
        SDL_memcpy(game.unlock, p.unlock, sizeof(p.unlock));
        SDL_memcpy(game.unlocknotify, p.unlocknotify, sizeof(p.unlocknotify));
        SDL_memcpy(game.besttimes, p.besttimes, sizeof(p.besttimes));
        SDL_memcpy(game.bestframes, p.bestframes, sizeof(p.bestframes));
        SDL_memcpy(game.besttrinkets, p.besttrinkets, sizeof(p.besttrinkets));
        SDL_memcpy(game.bestlives, p.bestlives, sizeof(p.bestlives));
        SDL_memcpy(game.bestrank, p.bestrank, sizeof(p.bestrank));
        game.stat_trinkets = p.stat_trinkets;
        game.bestgamedeaths = p.bestgamedeaths;
        game.swnbestrank = p.swnbestrank;
        game.swnrecord = p.swnrecord;
        game.customlevelstats = p.customlevelstats;
        map.invincibility = p.invincibility;
    }

    static void gfx_capture(GfxState& g)
    {
        g.rcol = graphics.rcol;
        g.towerbg = graphics.towerbg;
        g.fademode = graphics.fademode;
        g.fadeamount = graphics.fadeamount;
        g.oldfadeamount = graphics.oldfadeamount;
        SDL_memcpy(g.fadebars, graphics.fadebars, sizeof(g.fadebars));
        g.ingame_fademode = graphics.ingame_fademode;
        g.showcutscenebars = graphics.showcutscenebars;
        g.cutscenebarspos = graphics.cutscenebarspos;
        g.oldcutscenebarspos = graphics.oldcutscenebarspos;
        g.trinketcolset = graphics.trinketcolset;
        g.trinketr = graphics.trinketr;
        g.trinketg = graphics.trinketg;
        g.trinketb = graphics.trinketb;
        g.linestate = graphics.linestate;
        g.linedelay = graphics.linedelay;
        g.backoffset = graphics.backoffset;
        g.textboxes = graphics.textboxes;
    }

    static void gfx_apply(const GfxState& g)
    {
        /* The texture belongs to the graphics system, so keep the live one. */
        SDL_Texture* live_texture = graphics.towerbg.texture;

        graphics.rcol = g.rcol;
        graphics.towerbg = g.towerbg;
        graphics.towerbg.texture = live_texture;
        graphics.fademode = g.fademode;
        graphics.fadeamount = g.fadeamount;
        graphics.oldfadeamount = g.oldfadeamount;
        SDL_memcpy(graphics.fadebars, g.fadebars, sizeof(g.fadebars));
        graphics.ingame_fademode = g.ingame_fademode;
        graphics.showcutscenebars = g.showcutscenebars;
        graphics.cutscenebarspos = g.cutscenebarspos;
        graphics.oldcutscenebarspos = g.oldcutscenebarspos;
        graphics.trinketcolset = g.trinketcolset;
        graphics.trinketr = g.trinketr;
        graphics.trinketg = g.trinketg;
        graphics.trinketb = g.trinketb;
        graphics.linestate = g.linestate;
        graphics.linedelay = g.linedelay;
        graphics.backoffset = g.backoffset;
        graphics.textboxes = g.textboxes;

        /* Make the background and tower background redraw. */
        graphics.backgrounddrawn = false;
        graphics.foregrounddrawn = false;
        graphics.towerbg.tdrawback = true;
    }

    struct Slot
    {
        Game* game;
        mapclass* map;
        entityclass* obj;
        scriptclass* script;
        std::string roomname;
        GfxState gfx;
        int song;
    };

    static std::map<int, Slot> slots;
    static int last_ndm_key = -1;

    static int current_mode = -1;
    static bool loaded_this_run = false;
    static bool trial_super = false;
    static bool super_setting = false;
    static bool allow_setting = false;
    static bool pending_load = false;
    static bool restart_pending = false;
    static bool restart_instant = false;
    static bool restarting = false;
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

    static int make_key(const int mode, const bool super)
    {
        return mode * 2 + (super ? 1 : 0);
    }

    static int current_key(void)
    {
        return make_key(current_mode, trial_super);
    }

    static void free_slot(Slot& slot)
    {
        delete slot.game;
        delete slot.map;
        delete slot.obj;
        delete slot.script;
    }

    static void show_popup(const char* text)
    {
        popup_text = text;
        popup_timer = POPUP_FRAMES;
    }

    static bool ndm_state_exists(void)
    {
        for (std::map<int, Slot>::const_iterator it = slots.begin(); it != slots.end(); ++it)
        {
            if (is_ndm_mode(it->first / 2))
            {
                return true;
            }
        }
        return false;
    }

    static bool run_has_state(void)
    {
        if (is_ndm_mode(current_mode))
        {
            return ndm_state_exists();
        }
        return slots.count(current_key()) != 0;
    }

    static bool active(void)
    {
        return current_mode != -1
            && game.gamestate == GAMEMODE
            && (game.intimetrial || game.inintermission || game.nodeathmode)
            && !game.translator_exploring
            && !map.custommode
            && !game.fadetomenu
            && !game.fadetolab;
    }

    static void save(void)
    {
        /* Don't capture the death animation. */
        if (game.deathseq != -1)
        {
            return;
        }

        const int key = current_key();
        std::map<int, Slot>::iterator old = slots.find(key);
        if (old != slots.end())
        {
            free_slot(old->second);
            slots.erase(old);
        }

        Slot slot;
        slot.game = new Game(game);
        slot.map = new mapclass(map);
        slot.obj = new entityclass(obj);
        slot.script = new scriptclass(script);
        slot.roomname = map.roomname;
        gfx_capture(slot.gfx);
        slot.song = music.currentsong;
        slots[key] = slot;

        if (is_ndm_mode(current_mode))
        {
            last_ndm_key = key;
        }
        show_popup("State saved");
    }

    static void restore_music(const int song)
    {
        if (music.currentsong == song)
        {
            return;
        }

        if (song == -1)
        {
            music.haltdasmusik();
            return;
        }

        int track = song;
        if (music.mmmmmm && !music.usingmmmmmm)
        {
            track -= music.num_mmmmmm_tracks;
        }
        if (track < 0)
        {
            return;
        }

        music.nicefade = false;
        music.nicechange = -1;
        music.play(track);
    }

    static bool load(void)
    {
        std::map<int, Slot>::iterator it = slots.find(current_key());
        if (it == slots.end())
        {
            return false;
        }
        const Slot& slot = it->second;

        Persist keep;
        persist_capture(keep);

        game = *slot.game;
        map = *slot.map;
        obj = *slot.obj;
        script = *slot.script;

        persist_apply(keep);
        gfx_apply(slot.gfx);

        /* map.roomname points into a shared buffer, so rebuild it. */
        map.setroomname(slot.roomname.c_str());

        /* Holding the flip button while loading shouldn't flip. */
        game.jumpheld = true;
        game.jumppressed = 0;

        restore_music(slot.song);

        loaded_this_run = true;
        show_popup("State loaded");
        return true;
    }

    void note_mode(enum StartMode mode)
    {
        current_mode = (int) mode;
        loaded_this_run = false;
        restart_pending = false;
        restart_instant = false;
        trial_super = super_setting && is_trial_mode(current_mode);
        if (!restarting)
        {
            popup_timer = 0;
        }
    }

    void after_start(void)
    {
        if (!pending_load)
        {
            return;
        }

        pending_load = false;
        if (!load())
        {
            show_popup("No saved state");
        }
    }

    bool invalid_run(void)
    {
        return loaded_this_run || trial_super || run_has_state();
    }

    bool gg_level_complete(void)
    {
        return invalid_run() && (game.intimetrial || game.inintermission);
    }

    bool gg_game_complete(void)
    {
        return invalid_run();
    }

    void apply_trial_mode(void)
    {
        if (trial_super)
        {
            game.nodeathmode = true;
            game.nocutscenes = true;
        }
    }

    void apply_trial_countdown(void)
    {
        if (trial_super)
        {
            game.timetrialcountdown = TRIAL_COUNTDOWN;
        }
    }

    GameOverButtons gameover_buttons(void)
    {
        GameOverButtons buttons;
        buttons.load = allow_setting && last_ndm_key != -1 && slots.count(last_ndm_key) != 0
            && is_ndm_mode(current_mode);
        buttons.retry = buttons.load;
        return buttons;
    }

    enum StartMode retry_mode(void)
    {
        return (enum StartMode) current_mode;
    }

    enum StartMode state_mode(void)
    {
        return (enum StartMode) (last_ndm_key / 2);
    }

    void request_load(void)
    {
        pending_load = true;
    }

    bool fast_death_exit(void)
    {
        if (!super_setting
        || !game.nodeathmode
        || game.deathseq == -1
        || game.deathseq > 30 - FAST_DEATH_FRAMES)
        {
            return false;
        }

        if (game.intimetrial || game.inintermission)
        {
            if (!restart_pending)
            {
                restart_pending = true;
                restart_instant = true;
            }
            return true;
        }

        game.gethardestroom();
        game.copyndmresults();
        obj.entities.clear();
        game.quittomenu();
        graphics.fademode = FADE_NONE;

        if (gameover_buttons().retry)
        {
            game.createmenu(Menu::gameover2);
        }
        else
        {
            game.createmenu(Menu::startnodeathmode);
            game.currentmenuoption = current_mode == Start_NODEATHMODE_WITHCUTSCENES ? 1 : 0;
            music.play(Music_PRESENTINGVVVVVV);
        }
        return true;
    }

    bool popup_active(void)
    {
        return popup_timer > 0;
    }

    const char* popup_message(void)
    {
        return popup_text.c_str();
    }

    bool options_has_super(void)
    {
        return game.ingame_titlemode
            && current_mode != -1
            && (game.intimetrial || game.inintermission || game.nodeathmode)
            && !map.custommode
            && !game.translator_exploring;
    }

    bool super_mode(void)
    {
        return super_setting;
    }

    bool allow_states(void)
    {
        return allow_setting;
    }

    int state_count(void)
    {
        return (int) slots.size();
    }

    void toggle_super(void)
    {
        super_setting = !super_setting;
        show_popup(super_setting ? "Super mode on" : "Super mode off");
    }

    void toggle_allow(void)
    {
        allow_setting = !allow_setting;
        show_popup(allow_setting ? "Savestates on" : "Savestates off");
    }

    void delete_states(void)
    {
        for (std::map<int, Slot>::iterator it = slots.begin(); it != slots.end(); ++it)
        {
            free_slot(it->second);
        }
        slots.clear();
        last_ndm_key = -1;
        show_popup("Savestates deleted");
    }

    static void begin_restart(void)
    {
        graphics.fademode = FADE_START_FADEOUT;
        game.completestop = true;
        music.fadeout();
        restart_pending = true;
        restart_instant = false;
    }

    static void update_restart(void)
    {
        if (!restart_pending)
        {
            return;
        }
        if (!restart_instant && graphics.fademode != FADE_FULLY_BLACK)
        {
            return;
        }

        const bool instant = restart_instant;
        restart_pending = false;
        restart_instant = false;
        restarting = true;
        script.startgamemode((enum StartMode) current_mode);
        restarting = false;
        game.deathseq = -1;
        game.completestop = false;
        if (instant)
        {
            graphics.fademode = FADE_NONE;
        }
        if (game.intimetrial)
        {
            game.hascontrol = false;
        }
    }

    void input(void)
    {
        static bool was_down = false;

        if (popup_timer > 0)
        {
            popup_timer--;
        }

        update_restart();

        /* Super mode was toggled in the options menu during a trial. */
        if (active()
        && is_trial_mode(current_mode)
        && (super_setting != trial_super)
        && graphics.fademode == FADE_NONE
        && !restart_pending)
        {
            begin_restart();
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        const bool down = keystate[SDL_SCANCODE_GRAVE] != 0;
        const bool pressed = down && !was_down;
        was_down = down;

        if (!pressed || !active())
        {
            return;
        }

        const int mods = SDL_GetModState();
        if ((mods & (KMOD_CTRL | KMOD_ALT)) != 0)
        {
            return;
        }

        if (!allow_setting)
        {
            return;
        }

        if ((mods & KMOD_SHIFT) != 0)
        {
            save();
        }
        else if (!load())
        {
            show_popup("No saved state");
        }
    }
}
