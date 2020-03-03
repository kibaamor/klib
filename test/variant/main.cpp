#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <kstream.h>
#include <kvariant.h>
#include <kserializer.h>


TEST_SUITE_BEGIN("variant");
using namespace klib;

TEST_CASE("std::map<int32_t, std::vector<std::string>>")
{
    memstream m;
    variant a;
    variant b;

    {
        std::map<int32_t, std::vector<std::string>> data = {
            {0,{"a", "b", "c"}},
            {1,{"aa", "bb", "cc"}},
        };
        a.set(data);
        wserializer s(m);
        CHECK((s & a));
    }
    {
        rserializer s(m);
        CHECK((s & b));
    }

    CHECK(0 == m.read_size());
    CHECK(a == b);
}

TEST_CASE("string")
{
    std::string a = "variant serialize test";
    variant v = to_variant(a);

    std::string b;
    CHECK(from_variant(v, b));
    CHECK(a == b);
}

TEST_CASE("variant::object_t")
{
    variant::object_t a = {
        {0,"a"},
        {1,"aa"},
    };
    variant v = to_variant(a);

    variant::object_t b;
    CHECK(from_variant(v, b));
    CHECK(a == b);
}

TEST_CASE("std::map<int, std::string>")
{
    std::map<int, std::string> a = {
        {0,"a"},
        {1,"aa"},
    };
    variant v = to_variant(a);

    std::map<int, std::string> b;
    CHECK(from_variant(v, b));
    CHECK(a == b);
}

TEST_CASE("std::map<int32_t, std::vector<std::string>>")
{
    std::map<int32_t, std::vector<std::string>> a = {
        {0,{"a", "b", "c"}},
        {1,{"aa", "bb", "cc"}},
    };
    variant v = to_variant(a);

    std::map<int32_t, std::vector<std::string>> b;
    CHECK(from_variant(v, b));
    CHECK(a == b);
}

TEST_SUITE_END();
