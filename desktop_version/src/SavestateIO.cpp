#include "SavestateIO.h"

#include <set>

namespace savestate_io
{
    Stream::Stream(void)
        : in(NULL), length(0), position(0), is_reading(false), good(true)
    {
    }

    Stream::Stream(const unsigned char* data, const size_t size)
        : in(data), length(size), position(0), is_reading(true), good(true)
    {
    }

    bool Stream::reading(void) const
    {
        return is_reading;
    }

    bool Stream::ok(void) const
    {
        return good;
    }

    void Stream::fail(void)
    {
        good = false;
    }

    size_t Stream::remaining(void) const
    {
        return length - position;
    }

    void Stream::raw(void* value, const size_t size)
    {
        if (!is_reading)
        {
            const unsigned char* bytes = (const unsigned char*) value;
            out.insert(out.end(), bytes, bytes + size);
            return;
        }

        if (!good || size > remaining())
        {
            good = false;
            SDL_memset(value, 0, size);
            return;
        }
        SDL_memcpy(value, in + position, size);
        position += size;
    }

    const unsigned char* Stream::data(void) const
    {
        return out.empty() ? NULL : &out[0];
    }

    size_t Stream::size(void) const
    {
        return out.size();
    }

    void io(Stream& s, std::string& v)
    {
        unsigned int count = (unsigned int) v.size();
        io(s, count);
        if (!s.reading())
        {
            if (count != 0)
            {
                s.raw(&v[0], count);
            }
            return;
        }

        if (count > s.remaining())
        {
            s.fail();
            return;
        }
        v.assign(count, '\0');
        if (count != 0)
        {
            s.raw(&v[0], count);
        }
    }

    void io_cstr(Stream& s, const char*& v)
    {
        static std::set<std::string> kept;

        bool present = v != NULL;
        io(s, present);
        if (!present)
        {
            v = NULL;
            return;
        }

        std::string text;
        if (!s.reading())
        {
            text = v;
        }
        io(s, text);
        if (s.reading())
        {
            v = kept.insert(text).first->c_str();
        }
    }
}
