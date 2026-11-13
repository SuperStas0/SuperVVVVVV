#ifndef SUPERMODE_H
#define SUPERMODE_H

#include "Script.h"

/* Super mode and the rules around savestates.
 *
 * Super mode off: death in No Death Mode loads the saved state after the
 * normal delay and starts a short countdown.
 * Super mode on: the delay is 0.5 s and there is no countdown. Without a state,
 * No Death Mode leaves to the cutscene choice screen. Time trials and
 * intermission replays also get No Death Mode rules, and restart the level on
 * death when there is no state.
 *
 * A run is invalid (GG!, no records, no unlocks) when a state was loaded in it,
 * a state exists for its mode, or Super mode is on in a time trial or
 * intermission replay. */
namespace supermode
{
    /* Called from savestate::note_mode(). */
    void note_mode(enum StartMode mode);

    /* Slot key of the current run. */
    int key(void);

    /* The state keys work right now. */
    bool states_allowed(void);

    /* The countdown after loading a state is running. */
    bool counting_down(void);
    void cancel_countdown(void);

    bool super_mode(void);
    bool allow_states(void);
    void toggle_super(void);
    void toggle_allow(void);
    void delete_states(void);

    bool invalid_run(void);

    /* Called from startgamemode() right after hardreset(), before the level is
     * created. Sets No Death Mode rules in time trials and intermission replays
     * with Super mode on. */
    void apply_trial_rules(void);

    /* Called around the time trial record code. In an invalid run the records
     * are put back, so nothing is written. */
    void protect_records(void);
    void restore_records(void);

    /* Called once per frame from savestate::input(). */
    void update(void);

    /* Called from the death logic after deathseq was decreased. Returns true if
     * it took over, so the original handling must be skipped. */
    bool death_exit(void);

    /* Dying will load the saved state of the run. The death then leaves the
     * music alone. */
    bool loads_state_on_death(void);

    /* Message in place of the room name. text is a key of supertext. */
    void popup(const char* text);
    bool render_popup(void);

    void render_countdown(void);

    /* Draws "GG!" centered on the screen. scale is 3 or 4. */
    void print_gg(int scale, int y, int r, int g, int b);

    /* Draws text only in a valid run. */
    void print_wrap_valid(uint32_t flags, int x, int y, const char* text, int r, int g, int b);

    /* Draws a screen title, or "GG!" in its place in an invalid run. */
    void print_title(int scale, uint32_t flags, int y, const char* text, int r, int g, int b);

    /* Draws "GG!" over the Level Complete and Game Complete images. */
    bool draw_gg(int image, bool flipmode);
}

#endif /* SUPERMODE_H */
