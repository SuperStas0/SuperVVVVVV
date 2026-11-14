#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "Script.h"

/* Savestates, SLOTS slots per key, kept in memory and in saves/states.vvv.
 * Keys come from supermode::key().
 *   1 2 3            load slot 1 2 3
 *   Shift + 1 2 3    save slot 1 2 3 */
namespace savestate
{
    enum { SLOTS = 3 };

    /* Called whenever a game mode starts. */
    void note_mode(enum StartMode mode);

    /* Called once per frame from gameinput(). */
    void input(void);

    /* True if a state was loaded during the current run. */
    bool used(void);

    bool exists(int key, int slot);
    bool exists_any(int key);

    /* The slot of key that was saved or loaded last, -1 if none. */
    int last(int key);

    /* Number of saved states in all slots of all keys. */
    int count(void);
    void remove_all(void);

    /* Returns false if there is nothing to save right now. */
    bool save(int key, int slot);

    /* Returns false if the slot is empty. The music restarts only if the
     * saved song is not the one playing. */
    bool load(int key, int slot);
}

#endif /* SAVESTATE_H */
