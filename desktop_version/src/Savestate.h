#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "Script.h"

/* In-memory savestate for practicing time trials, intermission replays and
 * No Death Mode. One slot.
 *   `            load
 *   Shift + `    save
 *   Ctrl + `     toggle NDM mode in time trials and intermission replays
 *   Alt + `      toggle fast death in No Death Mode */
namespace savestate
{
    struct GameOverButtons
    {
        bool load;
        bool retry;
    };

    /* Called whenever a game mode starts. */
    void note_mode(enum StartMode mode);

    /* Called at the end of startgamemode(). Applies a state requested from
     * the game over screen. */
    void after_start(void);

    /* Called once per frame from gameinput(). */
    void input(void);

    /* True if a state was loaded during the current run. */
    bool used(void);

    /* True if the current run must not count as a record or unlock anything:
     * a state was loaded, or NDM mode is on in a time trial or intermission. */
    bool invalid_run(void);

    /* "GG!" replaces the Level Complete / Game Complete graphic. */
    bool gg_level_complete(void);
    bool gg_game_complete(void);

    /* Sets NDM flags for time trials and intermissions when NDM mode is on. */
    void apply_trial_mode(void);

    /* Called from the death logic. Leaves the game early when fast death is
     * on. Returns true if it did. */
    bool fast_death_exit(void);

    /* Extra buttons of the game over screen. */
    GameOverButtons gameover_buttons(void);
    enum StartMode retry_mode(void);
    enum StartMode state_mode(void);
    void request_load(void);

    /* Message shown in place of the room name. */
    bool popup_active(void);
    const char* popup_message(void);
}

#endif /* SAVESTATE_H */
