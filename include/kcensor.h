#pragma once
#include <cstddef>
#include <string>

namespace klib {

class censor {
public:
    censor()
    {
        _root = new node();
    }
    ~censor()
    {
        delete _root;
    }
    censor(const censor&) = delete;
    censor& operator=(const censor&) = delete;

    void add_word(const std::string& word);
    bool has_word(const std::string& sentence) const noexcept;
    bool filter_word(std::string& sentence, char replace = '*') const noexcept;

private:
    bool is_word(const std::string& word, size_t pos, size_t* end = nullptr) const noexcept;

private:
    struct node {
        node* nodes[0xff] = {};
        bool isword = false;

        ~node()
        {
            for (auto n : nodes) {
                if (nullptr != n)
                    delete n;
            }
        }
    };
    node* _root = nullptr;
};

} // namespace klib
