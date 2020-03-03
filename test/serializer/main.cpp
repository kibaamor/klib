#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <kserializer.h>
#include <climits>
#include <cfloat>


struct SubData
{
    friend class klib::serialization_access;

    std::string _str;

    template <typename S>
    bool serialize(S& s)
    {
        return (s & _str);
    }

public:
    void init()
    {
        _str = "string data";
    }

    std::string to_string() const
    {
        return std::string("str: ") + _str;
    }

    friend bool operator==(const SubData& lhs, const SubData& rhs)
    {
        return lhs._str == rhs._str;
    }
};

class Data
{
    friend class klib::serialization_access;

    template <typename S>
    bool serialize(S& s)
    {
        return (s & _b)
            && (s & _i8)
            && (s & _u8)
            && (s & _i16)
            && (s & _u16)
            && (s & _i32)
            && (s & _u32)
            && (s & _i64)
            && (s & _u64)
            && (s & _f32)
            && (s & _f64)
            && (s & _str)
            && (s & _data)
            && (s & _sub);
    }

    bool _b = false;
    int8_t _i8 = 0;
    uint8_t _u8 = 0;
    int16_t _i16 = 0;
    uint16_t _u16 = 0;
    int32_t _i32 = 0;
    uint32_t _u32 = 0;
    int64_t _i64 = 0;
    uint64_t _u64 = 0;
    float _f32 = 0.0f;
    double _f64 = 0.0;
    std::string _str;
    std::map<int32_t, std::vector<std::string>> _data;
    SubData _sub;

public:
    void init()
    {
        _b = true;
        _i8 = 'a';
        _u8 = 100;
        _i16 = INT16_MIN;
        _u16 = UINT16_MAX;
        _i32 = INT32_MIN;
        _u32 = UINT32_MAX;
        _i64 = INT64_MIN;
        _u64 = UINT64_MAX;
        _f32 = FLT_MAX;
        _f64 = DBL_MAX;
        _str = "kom";
        _data = {
            {0,{"a", "b", "c"}},
            {1,{"aa", "bb", "cc"}},
        };
        _sub.init();
    }

    std::string to_string() const
    {
        return std::string("bool: ") + (_b ? "true" : "false")
            + " i8: " + std::to_string(_i8)
            + " u8: " + std::to_string(_u8)
            + " i16: " + std::to_string(_i16)
            + " u16: " + std::to_string(_u16)
            + " i32: " + std::to_string(_i32)
            + " u32: " + std::to_string(_u32)
            + " i64: " + std::to_string(_i64)
            + " u64: " + std::to_string(_u64)
            + " f32: " + std::to_string(_f32)
            + " f64: " + std::to_string(_f64)
            + " str: " + _str
            + " data: " + klib::variant(_data).to_string()
            + " sub: " + _sub.to_string();
    }

    friend bool operator==(const Data& lhs, const Data& rhs)
    {
        return lhs._b == rhs._b
            && lhs._i8 == rhs._i8
            && lhs._u8 == rhs._u8
            && lhs._i16 == rhs._i16
            && lhs._u16 == rhs._u16
            && lhs._i32 == rhs._i32
            && lhs._u32 == rhs._u32
            && lhs._i64 == rhs._i64
            && lhs._u64 == rhs._u64
            && lhs._f32 == rhs._f32
            && lhs._f64 == rhs._f64
            && lhs._str == rhs._str
            && lhs._data == rhs._data
            && lhs._sub == rhs._sub;
    }
};

inline bool operator!=(const Data& lhs, const Data& rhs)
{
    return !(lhs == rhs);
}

TEST_SUITE_BEGIN("serializer");
using namespace klib;

TEST_CASE("Data")
{
    memstream m;
    Data a;
    Data b;

    {
        a.init();
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

TEST_CASE("int64")
{
    memstream m;
    int64_t a;
    int64_t b;

    {
        a = 0x1234567890abcdef;
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

TEST_CASE("std::map<int32_t, std::vector<std::string>>")
{
    memstream m;
    std::map<int32_t, std::vector<std::string>> a;
    std::map<int32_t, std::vector<std::string>> b;

    {
        a = {
            {0,{"a", "b", "c"}},
            {1,{"aa", "bb", "cc"}},
        };
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

TEST_SUITE_END();
