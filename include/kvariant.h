#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <valarray>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <type_traits>


namespace klib
{
    enum class vtype_t : std::uint8_t
    {
        null = 0,
        boolean = 1,
        int8 = 2,
        uint8 = 3,
        int16 = 4,
        uint16 = 5,
        int32 = 6,
        uint32 = 7,
        int64 = 8,
        uint64 = 9,
        float32 = 10,
        float64 = 11,
        string = 12,
        array = 13,
        object = 14,
    };

    bool is_integral_vtype(vtype_t t) noexcept;
    bool is_floatpoint_vtype(vtype_t t) noexcept;
    const char* get_vtype_name(vtype_t t) noexcept;

    class variant final
    {
    public:
        using boolean_t = bool;
        using float32_t = float;
        using float64_t = double;
        using string_t = std::string;
        using array_t = std::vector<variant>;
        using object_t = std::map<variant, variant>;

        variant() noexcept : _type(vtype_t::null)
        {
            _value = {}; assert_valid();
        }
        variant(boolean_t v) noexcept : _type(vtype_t::boolean)
        {
            _value.b = v; assert_valid();
        }
        variant(int8_t v) noexcept : _type(vtype_t::int8)
        {
            _value.i8 = v; assert_valid();
        }
        variant(uint8_t v) noexcept : _type(vtype_t::uint8)
        {
            _value.u8 = v; assert_valid();
        }
        variant(int16_t v) noexcept : _type(vtype_t::int16)
        {
            _value.i16 = v; assert_valid();
        }
        variant(uint16_t v) noexcept : _type(vtype_t::uint16)
        {
            _value.u16 = v; assert_valid();
        }
        variant(int32_t v) noexcept : _type(vtype_t::int32)
        {
            _value.i32 = v; assert_valid();
        }
        variant(uint32_t v) noexcept : _type(vtype_t::uint32)
        {
            _value.u32 = v; assert_valid();
        }
        variant(int64_t v) noexcept : _type(vtype_t::int64)
        {
            _value.i64 = v; assert_valid();
        }
        variant(uint64_t v) noexcept : _type(vtype_t::uint64)
        {
            _value.u64 = v; assert_valid();
        }
        variant(float32_t v) noexcept : _type(vtype_t::float32)
        {
            _value.f32 = v; assert_valid();
        }
        variant(float64_t v) noexcept : _type(vtype_t::float64)
        {
            _value.f64 = v; assert_valid();
        }
        variant(string_t&& v) : _type(vtype_t::string)
        {
            _value.str = new string_t(std::move(v)); assert_valid();
        }
        variant(array_t&& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t(std::move(v)); assert_valid();
        }
        variant(object_t&& v) : _type(vtype_t::object)
        {
            _value.obj = new object_t(std::move(v)); assert_valid();
        }

        template <typename T, typename std::enable_if<
            std::is_constructible<string_t, T>::value, int>::type = 0>
            variant(T&& v) : _type(vtype_t::string)
        {
            _value.str = new string_t(std::forward<T>(v)); assert_valid();
        }
        template <typename T, typename std::enable_if<
            std::is_constructible<array_t, T>::value, int>::type = 0>
            variant(T&& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t(std::forward<T>(v)); assert_valid();
        }
        template <typename T, typename std::enable_if<
            std::is_constructible<object_t, T>::value, int>::type = 0>
            variant(T&& v) : _type(vtype_t::object)
        {
            _value.obj = new object_t(std::forward<T>(v)); assert_valid();
        }

        template <typename T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        variant(T v) noexcept : variant(static_cast<typename std::underlying_type<T>::type>(v))
        {
            assert_valid();
        }

        variant(const std::vector<bool>& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(v.size());
            for (const bool b : v)
                _value.arr->emplace_back(b);
            assert_valid();
        }
        template <typename T, typename A, typename std::enable_if<
            std::is_constructible<variant, T>::value, int>::type = 0>
            variant(const std::vector<T, A>& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(v.size());
            for (const auto& x : v)
                _value.arr->emplace_back(x);
            assert_valid();
        }
        template <typename T, typename A, typename std::enable_if<
            std::is_constructible<variant, T>::value, int>::type = 0>
            variant(const std::list<T, A>& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(v.size());
            for (const auto& x : v)
                _value.arr->emplace_back(x);
            assert_valid();
        }
        template <typename T, typename std::enable_if<
            std::is_constructible<variant, T>::value, int>::type = 0>
            variant(const std::valarray<T>& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(v.size());
            for (const auto& x : v)
                _value.arr->emplace_back(x);
            assert_valid();
        }
        template <typename T, std::size_t N, typename std::enable_if<
            std::is_constructible<variant, T>::value, int>::type = 0>
            variant(const std::array<T, N>& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(v.size());
            for (const auto& x : v)
                _value.arr->emplace_back(x);
            assert_valid();
        }
        template <typename T, std::size_t N, typename std::enable_if<
            std::is_constructible<variant, T>::value
            && !std::is_constructible<string_t, const T(&)[N]>::value, int>::type = 0>
            variant(const T(&v)[N]) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(N);
            for (const auto& x : v)
                _value.arr->emplace_back(x);
            assert_valid();
        }
        template <typename K, typename P, typename A, typename std::enable_if<
            std::is_constructible<variant, K>::value, int>::type = 0>
            variant(const std::set<K, P, A>& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(v.size());
            for (const auto& x : v)
                _value.arr->emplace_back(x);
            assert_valid();
        }
        template <typename K, typename H, typename E, typename A, typename std::enable_if<
            std::is_constructible<variant, K>::value, int>::type = 0>
            variant(const std::unordered_set<K, H, E, A>& v) : _type(vtype_t::array)
        {
            _value.arr = new array_t();
            _value.arr->reserve(v.size());
            for (const auto& x : v)
                _value.arr->emplace_back(x);
            assert_valid();
        }

        template <typename K, typename V, typename P, typename A, typename std::enable_if<
            std::is_constructible<variant, K>::value && std::is_constructible<variant, V>::value, int>::type = 0>
            variant(const std::map<K, V, P, A>& v) : _type(vtype_t::object)
        {
            _value.obj = new object_t();
            for (const auto& kv : v)
                _value.obj->emplace(kv);
            assert_valid();
        }
        template <typename K, typename V, typename H, typename E, typename A, typename std::enable_if<
            std::is_constructible<variant, K>::value && std::is_constructible<variant, V>::value, int>::type = 0>
            variant(const std::unordered_map<K, V, H, E, A>& v) : _type(vtype_t::object)
        {
            _value.obj = new object_t();
            for (const auto& kv : v)
                _value.obj->emplace(kv);
            assert_valid();
        }

        ~variant() noexcept
        {
            clear();
        }

        variant(const variant& other);
        variant& operator=(const variant& other);
        variant(variant&& other) noexcept;
        variant& operator=(variant&& other) noexcept;

        void clear() noexcept;

        operator vtype_t() const noexcept
        {
            return _type;
        }
        vtype_t get_type() const noexcept
        {
            return _type;
        }

        template <typename T>
        void set(T&& v)
        {
            assert_valid();
            *this = variant(std::forward<T>(v));
            assert_valid();
        }

        template <typename T, typename std::enable_if<std::is_same<T, bool>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.b;
        }
        template <typename T, typename std::enable_if<std::is_same<T, int8_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.i8;
        }
        template <typename T, typename std::enable_if<std::is_same<T, uint8_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.u8;
        }
        template <typename T, typename std::enable_if<std::is_same<T, int16_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.i16;
        }
        template <typename T, typename std::enable_if<std::is_same<T, uint16_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.u16;
        }
        template <typename T, typename std::enable_if<std::is_same<T, int32_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.i32;
        }
        template <typename T, typename std::enable_if<std::is_same<T, uint32_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.u32;
        }
        template <typename T, typename std::enable_if<std::is_same<T, int64_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.i64;
        }
        template <typename T, typename std::enable_if<std::is_same<T, uint64_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.u64;
        }
        template <typename T, typename std::enable_if<std::is_same<T, float32_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.f32;
        }
        template <typename T, typename std::enable_if<std::is_same<T, float64_t>::value, int>::type = 0>
        constexpr T get() const noexcept
        {
            return _value.f64;
        }
        template <typename T, typename std::enable_if<std::is_same<T, string_t>::value, int>::type = 0>
        T& get()
        {
            return *_value.str;
        }
        template <typename T, typename std::enable_if<std::is_same<T, array_t>::value, int>::type = 0>
        T& get()
        {
            return *_value.arr;
        }
        template <typename T, typename std::enable_if<std::is_same<T, object_t>::value, int>::type = 0>
        T& get()
        {
            return *_value.obj;
        }
        template <typename T, typename std::enable_if<std::is_same<T, string_t>::value, int>::type = 0>
        const T& get() const
        {
            return *_value.str;
        }
        template <typename T, typename std::enable_if<std::is_same<T, array_t>::value, int>::type = 0>
        const T& get() const
        {
            return *_value.arr;
        }
        template <typename T, typename std::enable_if<std::is_same<T, object_t>::value, int>::type = 0>
        const T& get() const
        {
            return *_value.obj;
        }

        void swap(variant& other) noexcept;
        std::string to_string() const;
        void assert_valid() const;

    private:
        vtype_t _type;
        union
        {
            boolean_t b;
            int8_t i8;
            uint8_t u8;
            int16_t i16;
            uint16_t u16;
            int32_t i32;
            uint32_t u32;
            int64_t i64;
            uint64_t u64;
            float32_t f32;
            float64_t f64;
            string_t* str;
            array_t* arr;
            object_t* obj;
        } _value;

        int64_t get_integral_value() const noexcept;
        float64_t get_floatpoint_value() const noexcept;

        friend bool operator==(const variant& lhs, const variant& rhs) noexcept;
        friend bool operator<(const variant& lhs, const variant& rhs) noexcept;
    };

    inline void swap(variant& lhs, variant& rhs) noexcept
    {
        lhs.swap(rhs);
    }
    inline std::string to_string(const variant& v)
    {
        return v.to_string();
    }

    bool operator==(const variant& lhs, const variant& rhs) noexcept;
    bool operator<(const variant& lhs, const variant& rhs) noexcept;
    inline bool operator!=(const variant& lhs, const variant& rhs) noexcept
    {
        return !(lhs == rhs);
    }
    inline bool operator<=(const variant& lhs, const variant& rhs) noexcept
    {
        return !(rhs < lhs);
    }
    inline bool operator>(const variant& lhs, const variant& rhs) noexcept
    {
        return !(lhs <= rhs);
    }
    inline bool operator>=(const variant& lhs, const variant& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    inline variant to_variant(variant::boolean_t v)
    {
        return variant(v);
    }
    inline variant to_variant(int8_t v)
    {
        return variant(v);
    }
    inline variant to_variant(uint8_t v)
    {
        return variant(v);
    }
    inline variant to_variant(int16_t v)
    {
        return variant(v);
    }
    inline variant to_variant(uint16_t v)
    {
        return variant(v);
    }
    inline variant to_variant(int32_t v)
    {
        return variant(v);
    }
    inline variant to_variant(uint32_t v)
    {
        return variant(v);
    }
    inline variant to_variant(int64_t v)
    {
        return variant(v);
    }
    inline variant to_variant(uint64_t v)
    {
        return variant(v);
    }
    inline variant to_variant(variant::float32_t v)
    {
        return variant(v);
    }
    inline variant to_variant(variant::float64_t v)
    {
        return variant(v);
    }
    inline variant to_variant(const variant::string_t& v)
    {
        return variant(v);
    }
    inline variant to_variant(const variant::array_t& v)
    {
        return variant(v);
    }
    inline variant to_variant(const variant::object_t& v)
    {
        return variant(v);
    }
    inline variant to_variant(variant::string_t&& v)
    {
        return variant(std::move(v));
    }
    inline variant to_variant(variant::array_t&& v)
    {
        return variant(std::move(v));
    }
    inline variant to_variant(variant::object_t&& v)
    {
        return variant(std::move(v));
    }

    template <typename T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
    inline variant to_variant(T v)
    {
        return variant(static_cast<typename std::underlying_type<T>::type>(v));
    }

    template <typename T, typename A, typename std::enable_if<
        !std::is_same<T, klib::variant>::value, int>::type = 0>
        inline variant to_variant(const std::vector<T, A>& v)
    {
        variant::array_t arr;
        for (const auto& x : v)
            arr.emplace_back(std::move(to_variant(x)));
        return arr;
    }
    template <typename T, typename A>
    inline variant to_variant(const std::list<T, A>& v)
    {
        variant::array_t arr;
        for (const auto& x : v)
            arr.emplace_back(std::move(to_variant(x)));
        return arr;
    }
    template <typename K, typename P, typename A>
    inline variant to_variant(std::set<K, P, A>& v)
    {
        variant::array_t arr;
        for (const auto& x : v)
            arr.emplace_back(std::move(to_variant(x)));
        return arr;
    }

    template <typename K, typename V, typename P, typename A, typename std::enable_if<
        !std::is_same<K, klib::variant>::value
        && !std::is_same<V, klib::variant>::value, int>::type = 0>
        inline variant to_variant(std::map<K, V, P, A>& v)
    {
        variant::object_t obj;
        for (const auto& x : v)
            obj.emplace(std::make_pair(std::move(x.first), std::move(x.second)));
        return obj;
    }

    bool from_variant(const variant& var, variant::boolean_t& v);
    bool from_variant(const variant& var, int8_t& v);
    bool from_variant(const variant& var, uint8_t& v);
    bool from_variant(const variant& var, int16_t& v);
    bool from_variant(const variant& var, uint16_t& v);
    bool from_variant(const variant& var, int32_t& v);
    bool from_variant(const variant& var, uint32_t& v);
    bool from_variant(const variant& var, int64_t& v);
    bool from_variant(const variant& var, uint64_t& v);
    bool from_variant(const variant& var, variant::float32_t& v);
    bool from_variant(const variant& var, variant::float64_t& v);
    bool from_variant(const variant& var, variant::string_t& v);
    bool from_variant(const variant& var, variant::array_t& v);
    bool from_variant(const variant& var, variant::object_t& v);

    template <typename T, typename A, typename std::enable_if<
        !std::is_same<T, klib::variant>::value, int>::type = 0>
        inline bool from_variant(const variant& var, std::vector<T, A>& v)
    {
        variant::array_t arr;
        if (!from_variant(var, arr))
            return false;
        for (const auto& x : arr)
        {
            T t;
            if (!from_variant(x, t))
                return false;
            v.emplace_back(t);
        }
        return true;
    }
    template <typename T, typename A>
    inline bool from_variant(const variant& var, std::list<T, A>& v)
    {
        variant::array_t arr;
        if (!from_variant(var, arr))
            return false;
        for (const auto& x : arr)
        {
            T t;
            if (!from_variant(x, t))
                return false;
            v.emplace_back(t);
        }
        return true;
    }
    template <typename K, typename P, typename A>
    inline bool from_variant(const variant& var, std::set<K, P, A>& v)
    {
        variant::array_t arr;
        if (!from_variant(var, arr))
            return false;
        for (const auto& x : arr)
        {
            K t;
            if (!from_variant(x, t))
                return false;
            v.emplace(t);
        }
        return true;
    }

    template <typename K, typename V, typename P, typename A, typename std::enable_if<
        !std::is_same<K, klib::variant>::value
        && !std::is_same<V, klib::variant>::value, int>::type = 0>
        inline bool from_variant(const variant& var, std::map<K, V, P, A>& v)
    {
        variant::object_t obj;
        if (!from_variant(var, obj))
            return false;
        for (const auto& x : obj)
        {
            K k;
            V t;
            if (!from_variant(x.first, k) || !from_variant(x.second, t))
                return false;
            v.emplace(std::make_pair(k, t));
        }
        return true;
    }
}
