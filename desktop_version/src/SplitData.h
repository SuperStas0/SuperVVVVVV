#ifndef SPLITDATA_H
#define SPLITDATA_H

/* Fixed-layout mirror of the game variables the LiveSplit autosplitter reads.
 * The autosplitter finds it by scanning for the "SUPERVVVVVV_SPLIT" marker. */
namespace splitdata
{
    /* Called once per frame from the main loop. */
    void update(void);
}

#endif /* SPLITDATA_H */
