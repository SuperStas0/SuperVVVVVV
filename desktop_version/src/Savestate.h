#ifndef SAVESTATE_H
#define SAVESTATE_H

#include <vector>

#include "Script.h"

/* Savestates, SLOTS slots per key, kept in memory and in saves/states.vvv.
 * Keys come from supermode::key().
 *   1 to 9, 0        load slot 1 to 10
 *   Shift + 1 to 9, 0  save slot 1 to 10
 *   `                load the state that was saved or loaded last in the run */
namespace savestate
{
    enum { SLOTS = 10 };

    /* Called whenever a game mode starts. */
    void note_mode(enum StartMode mode);

    /* Called once per frame from gameinput(). */
    void input(void);

    bool exists(int key, int slot);
    bool exists_any(int key);

    /* The slot of key that was saved or loaded last in the current run, -1 if
     * none. Starting a game, level, time trial or intermission clears it. */
    int last(int key);

    /* Number of saved states in all slots of all keys. */
    int count(void);
    void remove_all(void);

    /* The same for the states of the given keys only. */
    int count_keys(const std::vector<int>& keys);
    void remove_keys(const std::vector<int>& keys);

    /* Returns false if there is nothing to save right now. */
    bool save(int key, int slot);

    /* Returns false if the slot is empty. The music restarts only if the
     * saved song is not the one playing. */
    bool load(int key, int slot);
}

#endif /* SAVESTATE_H */
