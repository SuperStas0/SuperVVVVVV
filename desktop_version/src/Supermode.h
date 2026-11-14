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
 * Savestates were on at its start or were turned on during it (even if never
 * used), or Super mode is on in a time trial or intermission replay. States
 * that only exist in the files do not matter. */
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

    /* Called from the death sequence of No Death Mode instead of fading out
     * the music. */
    void death_music(void);

    /* Pause menu entry of Super mode in the main game and in No Death Mode.
     * Main game: deletes the save files, unlocks all play modes and goes to the
     * play menu without a fade. No Death Mode: leaves like a death without a
     * state, to the cutscene choice screen. Without the Game Complete trophy, a
     * confirmation page comes first, until it was agreed to once. */
    bool reset_run_available(void);
    const char* reset_run_text(void);

    /* Last page of the pause menu: 32, or 33 with the reset run entry. */
    int last_pause_page(void);

    /* Action key on the reset run entry (page 33) and on its confirmation
     * pages (36 and 37). */
    void pause_page_action(void);
    void render_reset_prompt(int selection_offset);

    /* Message in place of the room name. text is a key of supertext. */
    void popup(const char* text);

    /* text has the placeholder {n}, filled with number. */
    void popup_state(const char* text, int number);
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
