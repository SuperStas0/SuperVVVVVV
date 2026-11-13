#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "Script.h"

/* In-memory savestates, one slot per key. Keys come from supermode::key().
 *   `            load
 *   Shift + `    save */
namespace savestate
{
    /* Called whenever a game mode starts. */
    void note_mode(enum StartMode mode);

    /* Called once per frame from gameinput(). */
    void input(void);

    /* True if a state was loaded during the current run. */
    bool used(void);

    bool exists(int key);
    int count(void);
    void remove_all(void);

    /* Returns false if there is nothing to save right now. */
    bool save(int key);

    /* Returns false if the slot is empty. The music restarts only if the
     * saved song is not the one playing. */
    bool load(int key);
}

#endif /* SAVESTATE_H */
