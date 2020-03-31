#pragma once

namespace klib {

struct section {
    using range_t = unsigned int;
    range_t beg;
    range_t end;

    section(range_t v)
        : beg(v)
        , end(v)
    {
    }
    section(range_t a, range_t b)
        : beg(a)
        , end(b)
    {
    }
};

inline bool operator<(const section& lhs, const section& rhs)
{
    return lhs.end < rhs.beg;
}

inline bool operator>(const section& lhs, const section& rhs)
{
    return lhs.beg > rhs.end;
}

inline bool operator==(const section& lhs, const section& rhs)
{
    return !(lhs < rhs) && !(lhs > rhs);
}

} // namespace klib
