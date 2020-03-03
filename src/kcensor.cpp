#include "../include/kcensor.h"


namespace
{
    int utf8_width(const char* s) noexcept
    {
        const unsigned char c = static_cast<unsigned char>(*s);
        if (c >= 0 && c <= 127)
            return 1;
        else if ((c & 0xe0) == 0xc0)
            return 2;
        else if ((c & 0xf0) == 0xe0)
            return 3;
        else if ((c & 0xf8) == 0xf0)
            return 4;
        //else if ((c & 0xfc) == 0xf8)
        //   return 5;
        //else if ((c & 0xfe) == 0xfc)
        //   return 6;
        else
            return 0;
    }
}

namespace klib
{
    void censor::add_word(const std::string& word)
    {
        auto cur = _root;
        for (const auto w : word)
        {
            const auto c = static_cast<unsigned char>(w);
            auto& next = cur->nodes[c];
            if (nullptr == next)
                next = new node();
            cur = next;
        }
        cur->isword = true;
    }

    bool censor::has_word(const std::string& sentence) const noexcept
    {
        for (size_t i = 0, N = sentence.size(); i < N; )
        {
            if (is_word(sentence, i))
                return true;
            const int len = utf8_width(&sentence[i]);
            if (len <= 0)
                ++i;
            else
                i += static_cast<size_t>(len);
        }
        return false;
    }

    bool censor::filter_word(std::string& sentence, char replace) const noexcept
    {
        bool ret = false;
        size_t end = 0;
        for (size_t i = 0, N = sentence.size(); i < N; )
        {
            if (is_word(sentence, i, &end))
            {
                std::fill(&sentence[i], &sentence[end], replace);
                i = end;
                ret = true;
                continue;
            }
            const int len = utf8_width(&sentence[i]);
            if (len <= 0)
                ++i;
            else
                i += static_cast<size_t>(len);
        }
        return ret;
    }

    bool censor::is_word(const std::string& word, size_t pos, size_t* end) const noexcept
    {
        auto cur = _root;
        for (size_t N = word.size(); pos < N; ++pos)
        {
            const auto c = static_cast<unsigned char>(word[pos]);
            cur = cur->nodes[c];
            if (nullptr == cur)
                return false;
            if (cur->isword)
            {
                if (nullptr != end)
                    *end = pos + 1;
                return true;
            }
        }
        return false;
    }
}
