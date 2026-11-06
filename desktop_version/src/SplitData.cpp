#include "SplitData.h"

#include <stddef.h>
#include <stdint.h>
#include <string>

#include "Entity.h"
#include "Game.h"
#include "Map.h"
#include "Script.h"

/* Defined in Input.cpp. */
extern bool fadetomode;
extern enum StartMode gotomode;

namespace splitdata
{
#pragma pack(push, 1)
    struct SplitData
    {
        char magic[32];             /* offset   0: "SUPERVVVVVV_SPLIT", zero padded */
        int32_t frames;             /* offset  32: game.frames */
        int32_t seconds;            /* offset  36: game.seconds */
        int32_t minutes;            /* offset  40: game.minutes */
        int32_t hours;              /* offset  44: game.hours */
        int32_t gotomode;           /* offset  48: gotomode (Input.cpp) */
        int32_t timetrialcountdown; /* offset  52: game.timetrialcountdown */
        int32_t saveframes;         /* offset  56: game.saveframes */
        int32_t saveseconds;        /* offset  60: game.saveseconds */
        int32_t state;              /* offset  64: game.state */
        int32_t teleport_to_x;      /* offset  68: game.teleport_to_x */
        int32_t teleport_to_y;      /* offset  72: game.teleport_to_y */
        int32_t swnrank;            /* offset  76: game.swnrank */
        int32_t deathcount;         /* offset  80: game.deathcounts */
        int32_t timetrialshinytarget; /* offset 84: game.timetrialshinytarget */
        int32_t timetrialpar;       /* offset  88: game.timetrialpar */
        int32_t gamestate;          /* offset  92: game.gamestate */
        uint8_t fadetomode;         /* offset  96: fadetomode (Input.cpp) */
        uint8_t finalstretch;       /* offset  97: map.finalstretch */
        uint8_t nodeathmode;        /* offset  98: game.nodeathmode */
        uint8_t ingame_titlemode;   /* offset  99: game.ingame_titlemode */
        uint8_t collect[100];       /* offset 100: obj.collect */
        char firstTextLine[64];     /* offset 200: script.txt[0], zero terminated */
    };
#pragma pack(pop)

    static_assert(offsetof(SplitData, frames) == 32, "layout");
    static_assert(offsetof(SplitData, gotomode) == 48, "layout");
    static_assert(offsetof(SplitData, gamestate) == 92, "layout");
    static_assert(offsetof(SplitData, fadetomode) == 96, "layout");
    static_assert(offsetof(SplitData, collect) == 100, "layout");
    static_assert(offsetof(SplitData, firstTextLine) == 200, "layout");
    static_assert(sizeof(SplitData) == 264, "layout");

    /* Written as a char array so the marker exists only here, not as a second
     * copy in a string literal. */
    static volatile SplitData data = {
        {'S','U','P','E','R','V','V','V','V','V','V','_','S','P','L','I','T'}
    };

    void update(void)
    {
        data.frames = game.frames;
        data.seconds = game.seconds;
        data.minutes = game.minutes;
        data.hours = game.hours;
        data.gotomode = (int32_t) gotomode;
        data.timetrialcountdown = game.timetrialcountdown;
        data.saveframes = game.saveframes;
        data.saveseconds = game.saveseconds;
        data.state = game.state;
        data.teleport_to_x = game.teleport_to_x;
        data.teleport_to_y = game.teleport_to_y;
        data.swnrank = game.swnrank;
        data.deathcount = game.deathcounts;
        data.timetrialshinytarget = game.timetrialshinytarget;
        data.timetrialpar = game.timetrialpar;
        data.gamestate = (int32_t) game.gamestate;
        data.fadetomode = fadetomode;
        data.finalstretch = map.finalstretch;
        data.nodeathmode = game.nodeathmode;
        data.ingame_titlemode = game.ingame_titlemode;

        for (int i = 0; i < 100; ++i)
        {
            data.collect[i] = obj.collect[i];
        }

        static const std::string empty;
        const std::string& line = script.txt.empty() ? empty : script.txt[0];
        const size_t length = line.size() < 63 ? line.size() : 63;
        for (size_t i = 0; i < 64; ++i)
        {
            data.firstTextLine[i] = i < length ? line[i] : '\0';
        }
    }
}
