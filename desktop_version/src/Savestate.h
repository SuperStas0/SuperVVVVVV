#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "Script.h"

/* In-memory savestate for practicing time trials and intermission replays.
 * ` (backquote/tilde key) = load, Shift+` = save. One slot. */
namespace savestate
{
    /* Called whenever a game mode starts. */
    void note_mode(enum StartMode mode);

    /* Called once per frame from gameinput(). */
    void input(void);

    /* True if a state was loaded during the current run. Such runs
     * must not write records or unlock achievements. */
    bool used(void);
}

#endif /* SAVESTATE_H */
