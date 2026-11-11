#ifndef SUPERTEXT_H
#define SUPERTEXT_H

/* Translations of the texts added by SuperVVVVVV.
 * They come from lang/<code>/super.xml, in the format of strings.xml. */
namespace supertext
{
    /* Returns the translation, or eng if there is none. */
    const char* get(const char* eng);
}

#endif /* SUPERTEXT_H */
