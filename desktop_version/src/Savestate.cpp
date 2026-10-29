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
#include "Script.h"

/* Whole-object snapshots of the gameplay globals. Anything that is a user
 * setting or persistent progress (not part of the run) is kept as-is on load. */

namespace savestate
{
    static int current_mode = -1;
    static int saved_mode = -1;
    static bool loaded_this_run = false;

    static Game* s_game = NULL;
    static mapclass* s_map = NULL;
    static entityclass* s_obj = NULL;
    static scriptclass* s_script = NULL;
    static std::string s_roomname;

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
    static GfxState s_gfx;

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

    static bool active(void)
    {
        return current_mode != -1
            && game.gamestate == GAMEMODE
            && (game.intimetrial || game.inintermission)
            && !game.nodeathmode
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

        delete s_game;
        delete s_map;
        delete s_obj;
        delete s_script;
        s_game = new Game(game);
        s_map = new mapclass(map);
        s_obj = new entityclass(obj);
        s_script = new scriptclass(script);
        s_roomname = map.roomname;
        gfx_capture(s_gfx);
        saved_mode = current_mode;
    }

    static void load(void)
    {
        if (s_game == NULL || saved_mode != current_mode)
        {
            return;
        }

        Persist keep;
        persist_capture(keep);

        game = *s_game;
        map = *s_map;
        obj = *s_obj;
        script = *s_script;

        persist_apply(keep);
        gfx_apply(s_gfx);

        /* map.roomname points into a shared buffer, so rebuild it. */
        map.setroomname(s_roomname.c_str());

        /* Holding the flip button while loading shouldn't flip. */
        game.jumpheld = true;
        game.jumppressed = 0;

        loaded_this_run = true;
    }

    void note_mode(enum StartMode mode)
    {
        current_mode = (int) mode;
        loaded_this_run = false;
    }

    bool used(void)
    {
        return loaded_this_run;
    }

    void input(void)
    {
        static bool was_down = false;

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        const bool down = keystate[SDL_SCANCODE_GRAVE] != 0;
        const bool pressed = down && !was_down;
        was_down = down;

        if (!pressed || !active())
        {
            return;
        }

        if ((SDL_GetModState() & KMOD_SHIFT) != 0)
        {
            save();
        }
        else
        {
            load();
        }
    }
}
