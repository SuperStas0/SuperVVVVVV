#include "Savestate.h"

#include <SDL.h>
#include <map>
#include <string>
#include <vector>

#include "Entity.h"
#include "Enums.h"
#include "Alloc.h"
#include "FileSystemUtils.h"
#include "Game.h"
#include "Graphics.h"
#include "Map.h"
#include "Music.h"
#include "SavestateIO.h"
#include "Script.h"
#include "Supermode.h"
#include "Vlogging.h"

/* Whole-object snapshots of the gameplay globals. Anything that is a user
 * setting or persistent progress (not part of the run) is kept as-is on load. */

namespace savestate
{
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
        int frames, seconds, minutes, hours;
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
        p.frames = game.frames;
        p.seconds = game.seconds;
        p.minutes = game.minutes;
        p.hours = game.hours;
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
        game.frames = p.frames;
        game.seconds = p.seconds;
        game.minutes = p.minutes;
        game.hours = p.hours;
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
    static std::map<int, int> last_slots;
    static bool loaded_this_run = false;

    static int slot_index(const int key, const int slot)
    {
        return key * SLOTS + slot;
    }

    static bool valid_slot(const int slot)
    {
        return slot >= 0 && slot < SLOTS;
    }

    static void free_slot(Slot& slot)
    {
        delete slot.game;
        delete slot.map;
        delete slot.obj;
        delete slot.script;
    }

    using savestate_io::io;

    static const char* const STATES_FILE = "saves/states.vvv";
    static const unsigned int FILE_MAGIC = 0x54535653; /* "SVST" */
    static const unsigned int FILE_VERSION = 1;
    static bool file_loaded = false;

    static unsigned int checksum(const unsigned char* data, const size_t length)
    {
        unsigned int hash = 2166136261u;
        for (size_t i = 0; i < length; i++)
        {
            hash ^= data[i];
            hash *= 16777619u;
        }
        return hash;
    }

    static void fill_header(unsigned int (&header)[5])
    {
        header[0] = FILE_MAGIC;
        header[1] = FILE_VERSION;
        header[2] = savestate_io::fields_hash();
        header[3] = (unsigned int) sizeof(long);
        header[4] = (unsigned int) sizeof(size_t);
    }

    static void io_slot(savestate_io::Stream& s, Slot& slot)
    {
        io(s, *slot.game);
        io(s, *slot.map);
        io(s, *slot.obj);
        io(s, *slot.script);
        io(s, slot.roomname);
        io(s, slot.gfx);
        io(s, slot.song);
    }

    static void write_file(void)
    {
        if (!FILESYSTEM_isInit())
        {
            return;
        }

        if (slots.empty())
        {
            if (FILESYSTEM_isFile(STATES_FILE) && !FILESYSTEM_delete(STATES_FILE))
            {
                vlog_error("Error deleting %s", STATES_FILE);
            }
            return;
        }

        savestate_io::Stream s;
        unsigned int header[5];
        fill_header(header);
        io(s, header);

        unsigned int count = (unsigned int) slots.size();
        io(s, count);
        for (std::map<int, Slot>::iterator it = slots.begin(); it != slots.end(); ++it)
        {
            int index = it->first;
            io(s, index);
            io_slot(s, it->second);
        }

        unsigned int last_count = (unsigned int) last_slots.size();
        io(s, last_count);
        for (std::map<int, int>::iterator it = last_slots.begin(); it != last_slots.end(); ++it)
        {
            int key = it->first;
            int slot = it->second;
            io(s, key);
            io(s, slot);
        }

        unsigned int sum = checksum(s.data(), s.size());
        io(s, sum);

        if (!FILESYSTEM_saveFile(STATES_FILE, s.data(), s.size()))
        {
            vlog_error("Could not write %s", STATES_FILE);
        }
    }

    static bool parse_file(
        const unsigned char* mem,
        const size_t length,
        std::map<int, Slot>& loaded,
        std::map<int, int>& loaded_last
    ) {
        unsigned int stored;
        if (length < sizeof(stored))
        {
            return false;
        }
        SDL_memcpy(&stored, mem + length - sizeof(stored), sizeof(stored));
        if (checksum(mem, length - sizeof(stored)) != stored)
        {
            return false;
        }

        savestate_io::Stream s(mem, length - sizeof(stored));
        unsigned int header[5];
        unsigned int expected[5];
        io(s, header);
        fill_header(expected);
        if (!s.ok() || SDL_memcmp(header, expected, sizeof(header)) != 0)
        {
            return false;
        }

        unsigned int count = 0;
        io(s, count);
        if (!s.ok() || count > s.remaining())
        {
            return false;
        }
        for (unsigned int i = 0; i < count; i++)
        {
            int index = 0;
            io(s, index);
            if (!s.ok() || index < 0 || loaded.count(index) != 0)
            {
                return false;
            }

            Slot slot;
            slot.game = new Game(game);
            slot.map = new mapclass(map);
            slot.obj = new entityclass(obj);
            slot.script = new scriptclass(script);
            slot.song = -1;
            io_slot(s, slot);
            if (!s.ok())
            {
                free_slot(slot);
                return false;
            }
            loaded[index] = slot;
        }

        unsigned int last_count = 0;
        io(s, last_count);
        if (!s.ok() || last_count > s.remaining())
        {
            return false;
        }
        for (unsigned int i = 0; i < last_count; i++)
        {
            int key = 0;
            int slot = 0;
            io(s, key);
            io(s, slot);
            loaded_last[key] = slot;
        }

        return s.ok() && s.remaining() == 0;
    }

    /* Reads the states of the previous launches, once. A file that does not
     * fit this build or is damaged is ignored, and replaced on the next save. */
    static void read_file(void)
    {
        if (file_loaded || !FILESYSTEM_isInit())
        {
            return;
        }
        file_loaded = true;

        unsigned char* mem = NULL;
        size_t length = 0;
        FILESYSTEM_loadFileToMemory(STATES_FILE, &mem, &length);
        if (mem == NULL)
        {
            return;
        }

        std::map<int, Slot> loaded;
        std::map<int, int> loaded_last;
        if (parse_file(mem, length, loaded, loaded_last))
        {
            slots = loaded;
            last_slots = loaded_last;
        }
        else
        {
            vlog_warn("Ignoring %s, it does not match this version", STATES_FILE);
            for (std::map<int, Slot>::iterator it = loaded.begin(); it != loaded.end(); ++it)
            {
                free_slot(it->second);
            }
        }
        VVV_free(mem);
    }

    void note_mode(enum StartMode mode)
    {
        loaded_this_run = false;
        supermode::note_mode(mode);
    }

    bool used(void)
    {
        return loaded_this_run;
    }

    bool exists(const int key, const int slot)
    {
        read_file();
        return valid_slot(slot) && slots.count(slot_index(key, slot)) != 0;
    }

    bool exists_any(const int key)
    {
        for (int slot = 0; slot < SLOTS; slot++)
        {
            if (exists(key, slot))
            {
                return true;
            }
        }
        return false;
    }

    int last(const int key)
    {
        read_file();
        const std::map<int, int>::const_iterator it = last_slots.find(key);
        if (it == last_slots.end() || !exists(key, it->second))
        {
            return -1;
        }
        return it->second;
    }

    int count(void)
    {
        read_file();
        return (int) slots.size();
    }

    void remove_all(void)
    {
        read_file();
        for (std::map<int, Slot>::iterator it = slots.begin(); it != slots.end(); ++it)
        {
            free_slot(it->second);
        }
        slots.clear();
        last_slots.clear();
        write_file();
    }

    bool save(const int key, const int slot)
    {
        /* Don't capture the death animation. */
        if (!valid_slot(slot) || game.deathseq != -1)
        {
            return false;
        }

        read_file();

        const int index = slot_index(key, slot);
        std::map<int, Slot>::iterator old = slots.find(index);
        if (old != slots.end())
        {
            free_slot(old->second);
            slots.erase(old);
        }

        Slot data;
        data.game = new Game(game);
        data.map = new mapclass(map);
        data.obj = new entityclass(obj);
        data.script = new scriptclass(script);
        data.roomname = map.roomname;
        gfx_capture(data.gfx);
        data.song = music.currentsong;
        slots[index] = data;
        last_slots[key] = slot;
        write_file();
        return true;
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

    bool load(const int key, const int slot)
    {
        if (!valid_slot(slot))
        {
            return false;
        }

        read_file();
        std::map<int, Slot>::iterator it = slots.find(slot_index(key, slot));
        if (it == slots.end())
        {
            return false;
        }
        const Slot& state = it->second;

        Persist keep;
        persist_capture(keep);

        game = *state.game;
        map = *state.map;
        obj = *state.obj;
        script = *state.script;

        persist_apply(keep);
        gfx_apply(state.gfx);

        /* map.roomname points into a shared buffer, so rebuild it. */
        map.setroomname(state.roomname.c_str());

        /* Holding the flip button while loading shouldn't flip. */
        game.jumpheld = true;
        game.jumppressed = 0;

        restore_music(state.song);

        if (last(key) != slot)
        {
            last_slots[key] = slot;
            write_file();
        }
        loaded_this_run = true;
        supermode::cancel_countdown();
        return true;
    }

    void input(void)
    {
        static const SDL_Scancode keys[SLOTS] = {SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3};
        static bool was_down[SLOTS] = {false, false, false};

        supermode::update();

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        int pressed_slot = -1;
        for (int slot = 0; slot < SLOTS; slot++)
        {
            const bool down = keystate[keys[slot]] != 0;
            if (down && !was_down[slot])
            {
                pressed_slot = slot;
            }
            was_down[slot] = down;
        }

        if (pressed_slot == -1 || !supermode::states_allowed())
        {
            return;
        }

        const int mods = SDL_GetModState();
        if ((mods & (KMOD_CTRL | KMOD_ALT)) != 0)
        {
            return;
        }

        const int number = pressed_slot + 1;
        if ((mods & KMOD_SHIFT) != 0)
        {
            if (!supermode::counting_down() && save(supermode::key(), pressed_slot))
            {
                supermode::popup_state("State {n} saved", number);
            }
        }
        else if (load(supermode::key(), pressed_slot))
        {
            supermode::popup_state("State {n} loaded", number);
        }
        else
        {
            supermode::popup_state("State {n} empty", number);
        }
    }
}
