#ifndef SAVESTATEIO_H
#define SAVESTATEIO_H

#include <SDL.h>
#include <map>
#include <string>
#include <vector>

#include "Entity.h"
#include "Enums.h"
#include "Game.h"
#include "Graphics.h"
#include "Map.h"
#include "Script.h"
#include "Textbox.h"
#include "TowerBG.h"

namespace savestate
{
    struct GfxState
    {
        int rcol;
        TowerBG towerbg;
        enum FadeBars fademode;
        int fadeamount, oldfadeamount;
        int fadebars[15];
        enum FadeBars ingame_fademode;
        bool showcutscenebars;
        int cutscenebarspos, oldcutscenebarspos;
        bool trinketcolset;
        int trinketr, trinketg, trinketb;
        int linestate, linedelay;
        int backoffset;
        std::vector<textboxclass> textboxes;
    };
}

/* Reads or writes the savestate objects as bytes, member by member. The same
 * io() functions do both, depending on the stream. The functions for the game
 * classes are in SavestateFields.cpp. */
namespace savestate_io
{
    class Stream
    {
    public:
        /* Writer */
        Stream(void);

        /* Reader, the data must stay valid while the stream is used */
        Stream(const unsigned char* data, size_t length);

        bool reading(void) const;

        /* False after a read ran past the end or found bad data. */
        bool ok(void) const;
        void fail(void);

        size_t remaining(void) const;
        void raw(void* value, size_t length);

        const unsigned char* data(void) const;
        size_t size(void) const;

    private:
        std::vector<unsigned char> out;
        const unsigned char* in;
        size_t length;
        size_t position;
        bool is_reading;
        bool good;
    };

    /* Identifies the member list of the game classes. */
    unsigned int fields_hash(void);

    void io(Stream& s, Game& v);
    void io(Stream& s, mapclass& v);
    void io(Stream& s, entityclass& v);
    void io(Stream& s, scriptclass& v);
    void io(Stream& s, savestate::GfxState& v);

    template<class T>
    inline void io_raw(Stream& s, T& v)
    {
        s.raw(&v, sizeof(v));
    }

    inline void io(Stream& s, bool& v)
    {
        unsigned char byte = v ? 1 : 0;
        s.raw(&byte, 1);
        v = byte != 0;
    }

    inline void io(Stream& s, char& v) { io_raw(s, v); }
    inline void io(Stream& s, signed char& v) { io_raw(s, v); }
    inline void io(Stream& s, unsigned char& v) { io_raw(s, v); }
    inline void io(Stream& s, short& v) { io_raw(s, v); }
    inline void io(Stream& s, unsigned short& v) { io_raw(s, v); }
    inline void io(Stream& s, int& v) { io_raw(s, v); }
    inline void io(Stream& s, unsigned int& v) { io_raw(s, v); }
    inline void io(Stream& s, long& v) { io_raw(s, v); }
    inline void io(Stream& s, unsigned long& v) { io_raw(s, v); }
    inline void io(Stream& s, long long& v) { io_raw(s, v); }
    inline void io(Stream& s, unsigned long long& v) { io_raw(s, v); }
    inline void io(Stream& s, float& v) { io_raw(s, v); }
    inline void io(Stream& s, double& v) { io_raw(s, v); }

    void io(Stream& s, std::string& v);

    /* A string that is kept for the rest of the program, so the pointer stays
     * valid. NULL is kept as NULL. */
    void io_cstr(Stream& s, const char*& v);

    template<class E>
    inline void io_enum(Stream& s, E& v)
    {
        int number = (int) v;
        io(s, number);
        v = (E) number;
    }

    template<class T, size_t N>
    inline void io(Stream& s, T (&v)[N])
    {
        for (size_t i = 0; i < N; i++)
        {
            io(s, v[i]);
        }
    }

    template<class T>
    inline void io(Stream& s, std::vector<T>& v)
    {
        unsigned int count = (unsigned int) v.size();
        io(s, count);
        if (s.reading())
        {
            if (count > s.remaining())
            {
                s.fail();
                return;
            }
            v.clear();
            v.resize(count);
        }
        for (size_t i = 0; i < v.size() && s.ok(); i++)
        {
            io(s, v[i]);
        }
    }

    template<class K, class V>
    inline void io(Stream& s, std::map<K, V>& v)
    {
        unsigned int count = (unsigned int) v.size();
        io(s, count);
        if (s.reading())
        {
            if (count > s.remaining())
            {
                s.fail();
                return;
            }
            v.clear();
            for (unsigned int i = 0; i < count && s.ok(); i++)
            {
                K key = K();
                V value = V();
                io(s, key);
                io(s, value);
                v[key] = value;
            }
            return;
        }
        for (typename std::map<K, V>::iterator it = v.begin(); it != v.end(); ++it)
        {
            K key = it->first;
            io(s, key);
            io(s, it->second);
        }
    }
}

#endif /* SAVESTATEIO_H */
