#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "Script.h"

/* In-memory savestates for time trials, intermission replays and No Death
 * Mode. One slot per mode, kept until the game closes.
 *   `            load
 *   Shift + `    save
 * Super mode and the state toggle are in the in-game options menu. */
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

    /* True if the current run must not count as a record or unlock anything:
     * a state was loaded, a state exists for this mode, or Super mode is on
     * in a time trial or intermission. */
    bool invalid_run(void);

    /* "GG!" replaces the Level Complete / Game Complete graphic. */
    bool gg_level_complete(void);
    bool gg_game_complete(void);

    /* Sets NDM rules for time trials and intermissions when Super mode is on. */
    void apply_trial_mode(void);
    void apply_trial_countdown(void);

    /* Called from the death logic. With Super mode on, leaves the game 0.5 s
     * after dying (restarts trials, opens a menu in NDM). Returns true if it did. */
    bool fast_death_exit(void);

    /* Extra buttons of the game over screen. */
    GameOverButtons gameover_buttons(void);
    enum StartMode retry_mode(void);
    enum StartMode state_mode(void);
    void request_load(void);

    /* In-game options menu. */
    bool options_has_super(void);
    bool super_mode(void);
    bool allow_states(void);
    int state_count(void);
    void toggle_super(void);
    void toggle_allow(void);
    void delete_states(void);

    /* Message shown in place of the room name. */
    bool popup_active(void);
    const char* popup_message(void);
}

#endif /* SAVESTATE_H */
