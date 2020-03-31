#pragma once
#include "kstream.h"
#include "kvariant.h"
#include "kserialization.h"

namespace klib {

struct serializer {
    enum type : uint8_t {
        TYPE_BOOL = 0 << 4,
        TYPE_INT8 = 1 << 4,
        TYPE_UINT8 = 2 << 4,
        TYPE_INT16 = 3 << 4,
        TYPE_UINT16 = 4 << 4,
        TYPE_INT32 = 5 << 4,
        TYPE_UINT32 = 6 << 4,
        TYPE_FLOAT = 7 << 4,
        TYPE_DOUBLE = 8 << 4,
        TYPE_INT64 = 9 << 4,
        TYPE_STRING = 10 << 4,
        TYPE_BINARY = 11 << 4,
        TYPE_MAP = 12 << 4,
        TYPE_LIST = 13 << 4,
        TYPE_POINTER = 14 << 4, // = TYPE_UINT64
        TYPE_UINT64 = 15 << 4,
    };

    static const char* get_type_name(uint8_t t) noexcept;
};

class wserializer : public wserialization<wserializer>, public serializer {
public:
    wserializer(wstream& stream)
        : _stream(stream)
    {
    }
    ~wserializer() = default;

    bool save(bool v);
    bool save(int8_t v);
    bool save(uint8_t v);
    bool save(int16_t v);
    bool save(uint16_t v);
    bool save(int32_t v);
    bool save(uint32_t v);
    bool save(int64_t v);
    bool save(uint64_t v);
    bool save(float v);
    bool save(double v);
    bool save(const std::string& v);
    bool save(const variant& v);

    template <typename T, typename A>
    bool save(const std::vector<T, A>& v)
    {
        memstream ms;
        wserializer wms(ms);
        bool fail = false;
        for (const auto& x : v) {
            if (!wms.save(x)) {
                fail = true;
                break;
            }
        }
        if (fail)
            return false;

        return _stream.write(static_cast<uint8_t>(TYPE_LIST))
            && save(static_cast<uint32_t>(ms.read_size()))
            && save(static_cast<uint32_t>(v.size()))
            && _stream.write(ms.read_ptr(), ms.read_size());
    }

    template <typename K, typename V, typename P, typename A>
    bool save(const std::map<K, V, P, A>& v)
    {
        memstream ms;
        wserializer wms(ms);
        bool fail = false;
        for (const auto& x : v) {
            if (!wms.save(x.first) || !wms.save(x.second)) {
                fail = true;
                break;
            }
        }
        if (fail)
            return false;

        return _stream.write(static_cast<uint8_t>(TYPE_MAP))
            && save(static_cast<uint32_t>(ms.read_size()))
            && save(static_cast<uint32_t>(v.size()))
            && _stream.write(ms.read_ptr(), ms.read_size());
    }

    template <typename V>
    bool save(const V& v)
    {
        return wserialization<wserializer>::save(v);
    }

private:
    bool save_impl(uint8_t t, uint64_t v, uint8_t l);

private:
    wstream& _stream;
};

class rserializer : public rserialization<rserializer>, public serializer {
public:
    rserializer(rstream& stream)
        : _stream(stream)
    {
    }
    ~rserializer() = default;

    bool load(bool& v);
    bool load(int8_t& v);
    bool load(uint8_t& v);
    bool load(int16_t& v);
    bool load(uint16_t& v);
    bool load(int32_t& v);
    bool load(uint32_t& v);
    bool load(int64_t& v);
    bool load(uint64_t& v);
    bool load(float& v);
    bool load(double& v);
    bool load(std::string& v);
    bool load(variant& v);

    template <typename T, typename A>
    bool load(std::vector<T, A>& v)
    {
        uint8_t type;
        if (!peek_type(type) || TYPE_LIST != type)
            return false;
        if (!discard(sizeof(uint8_t)))
            return false;
        uint32_t size;
        uint32_t count;
        if (!load(size) || !load(count))
            return false;

        if (size > 0) {
            std::vector<uint8_t> mem;
            mem.resize(size);
            if (!_stream.read(&mem[0], size))
                return false;

            rbufstream ms(&mem[0], size);
            rserializer rms(ms);

            v.resize(count);

            auto iter = v.begin();
            for (uint32_t i = 0; i < count; ++i, ++iter) {
                auto& x = *iter;
                if (!rms.load(x))
                    return false;
            }
        }
        return true;
    }

    template <typename K, typename V, typename P, typename A>
    bool load(std::map<K, V, P, A>& v)
    {
        uint8_t type;
        if (!peek_type(type) || TYPE_MAP != type)
            return false;
        if (!discard(sizeof(uint8_t)))
            return false;
        uint32_t size;
        uint32_t count;
        if (!load(size) || !load(count))
            return false;

        if (size > 0) {
            std::vector<uint8_t> mem;
            mem.resize(size);
            if (!_stream.read(&mem[0], size))
                return false;

            rbufstream ms(&mem[0], size);
            rserializer rms(ms);

            for (uint32_t i = 0; i < count; ++i) {
                K k;
                V x;
                if (!rms.load(k) || !rms.load(x))
                    return false;
                v.emplace(std::move(k), std::move(x));
            }
        }
        return true;
    }

    template <typename V>
    bool load(V& v)
    {
        return rserialization<rserializer>::load(v);
    }

private:
    bool peek_type(uint8_t& t);
    bool discard(size_t size)
    {
        return _stream.discard(size);
    }
    bool load_impl(uint8_t t, uint64_t& v, uint8_t l);

private:
    rstream& _stream;
};

} // namespace klib
