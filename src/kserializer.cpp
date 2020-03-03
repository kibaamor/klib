#include "../include/kserializer.h"


namespace klib
{
    const char* serializer::get_type_name(uint8_t t) noexcept
    {
        switch (t)
        {
        case TYPE_BOOL: return "bool";
        case TYPE_INT8: return "int8";
        case TYPE_UINT8: return "uint8";
        case TYPE_INT16: return "int16";
        case TYPE_UINT16: return "uint16";
        case TYPE_INT32: return "int32";
        case TYPE_UINT32: return "uint32";
        case TYPE_FLOAT: return "float";
        case TYPE_DOUBLE: return "double";
        case TYPE_INT64: return "int64";
        case TYPE_STRING: return "string";
        case TYPE_BINARY: return "binary";
        case TYPE_MAP: return "map";
        case TYPE_LIST: return "list";
        case TYPE_POINTER: return "pointer";
        case TYPE_UINT64: return "uint64";
        }
        return "unknown";
    }

    bool wserializer::save(bool v)
    {
        return _stream.write<uint8_t>(v ? (TYPE_BOOL | 0x1) : TYPE_BOOL);
    }

    bool wserializer::save(int8_t v)
    {
        return save_impl(TYPE_INT8, v, sizeof(v));
    }
    bool wserializer::save(uint8_t v)
    {
        return save_impl(TYPE_UINT8, v, sizeof(v));
    }
    bool wserializer::save(int16_t v)
    {
        return save_impl(TYPE_INT16, v, sizeof(v));
    }
    bool wserializer::save(uint16_t v)
    {
        return save_impl(TYPE_UINT16, v, sizeof(v));
    }
    bool wserializer::save(int32_t v)
    {
        return save_impl(TYPE_INT32, v, sizeof(v));
    }
    bool wserializer::save(uint32_t v)
    {
        return save_impl(TYPE_UINT32, v, sizeof(v));
    }
    bool wserializer::save(int64_t v)
    {
        return save_impl(TYPE_INT64, v, sizeof(v));
    }
    bool wserializer::save(uint64_t v)
    {
        return save_impl(TYPE_UINT64, v, sizeof(v));
    }

    bool wserializer::save(float v)
    {
        return _stream.write<uint8_t>(TYPE_FLOAT) && _stream.write<float>(v);
    }
    bool wserializer::save(double v)
    {
        return _stream.write<uint8_t>(TYPE_DOUBLE) && _stream.write<double>(v);
    }

    bool wserializer::save(const std::string& v)
    {
        const uint32_t size = static_cast<uint32_t>(v.size());
        return save_impl(TYPE_STRING, size, sizeof(size))
            && (0 == size ? true : _stream.write(v.data(), size));
    }

    bool wserializer::save(const variant& v)
    {
        switch (v.get_type())
        {
        case vtype_t::null: return true;
        case vtype_t::boolean: return save(v.get<variant::boolean_t>());
        case vtype_t::int8: return save(v.get<int8_t>());
        case vtype_t::uint8: return save(v.get<uint8_t>());
        case vtype_t::int16: return save(v.get<int16_t>());
        case vtype_t::uint16: return save(v.get<uint16_t>());
        case vtype_t::int32: return save(v.get<int32_t>());
        case vtype_t::uint32: return save(v.get<uint32_t>());
        case vtype_t::int64: return save(v.get<int64_t>());
        case vtype_t::uint64: return save(v.get<uint64_t>());
        case vtype_t::float32: return save(v.get<variant::float32_t>());
        case vtype_t::float64: return save(v.get<variant::float64_t>());
        case vtype_t::string: return save(v.get<variant::string_t>());
        case vtype_t::array: return save(v.get<variant::array_t>());
        case vtype_t::object: return save(v.get<variant::object_t>());
        default: return false;
        }
    }

    bool wserializer::save_impl(uint8_t t, uint64_t v, uint8_t l)
    {
        uint8_t mask = 0;
        for (uint8_t i = 0; i < l; ++i)
        {
            const uint8_t offset = (i << 3);
            const uint64_t test = (static_cast<uint64_t>(0xff) << offset);
            const uint64_t result = (v & test);
            if (0 != result) mask |= (0x1 << i);
        }
        if (l <= 4)
        {
            if (!_stream.write<uint8_t>(t | mask))
                return false;
        }
        else
        {
            if (!_stream.write<uint8_t>(t) || !_stream.write<uint8_t>(mask))
                return false;
        }

        for (uint8_t i = 0; i < l; ++i)
        {
            const uint8_t offset = (i << 3);
            const uint64_t test = (static_cast<uint64_t>(0xff) << offset);
            const uint64_t result = (v & test);
            if (0 != result && !_stream.write<uint8_t>(static_cast<uint8_t>(result >> offset)))
                return false;
        }
        return true;
    }


    bool rserializer::load(bool& v)
    {
        uint8_t d = 0;
        if (!_stream.peek(d) || TYPE_BOOL != (d & 0xf0))
            return false;
        v = (0 != (d & 0xf));
        return _stream.discard(1);
    }

    bool rserializer::load(int8_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_INT8, d, sizeof(v)))
            return false;
        v = static_cast<int8_t>(d);
        return true;
    }
    bool rserializer::load(uint8_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_UINT8, d, sizeof(v)))
            return false;
        v = static_cast<uint8_t>(d);
        return true;
    }
    bool rserializer::load(int16_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_INT16, d, sizeof(v)))
            return false;
        v = static_cast<int16_t>(d);
        return true;
    }
    bool rserializer::load(uint16_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_UINT16, d, sizeof(v)))
            return false;
        v = static_cast<uint16_t>(d);
        return true;
    }
    bool rserializer::load(int32_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_INT32, d, sizeof(v)))
            return false;
        v = static_cast<int32_t>(d);
        return true;
    }
    bool rserializer::load(uint32_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_UINT32, d, sizeof(v)))
            return false;
        v = static_cast<uint32_t>(d);
        return true;
    }
    bool rserializer::load(int64_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_INT64, d, sizeof(v)))
            return false;
        v = static_cast<int64_t>(d);
        return true;
    }
    bool rserializer::load(uint64_t& v)
    {
        uint64_t d = 0;
        if (!load_impl(TYPE_UINT64, d, sizeof(v)))
            return false;
        v = static_cast<uint64_t>(d);
        return true;
    }

    bool rserializer::load(float& v)
    {
        uint8_t d = 0;
        if (!_stream.peek(d) || TYPE_FLOAT != (d & 0xf0))
            return false;
        union
        {
#pragma pack(push)
#pragma pack(1)
            struct
            {
                uint8_t t;
                float f;
            } s;
#pragma pack(pop)
            uint8_t b[1 + sizeof(float)] = {};
        } tmp;
        if (!_stream.read(tmp.b, sizeof(tmp.b)))
            return false;
        v = tmp.s.f;
        return true;
    }
    bool rserializer::load(double& v)
    {
        uint8_t d = 0;
        if (!_stream.peek(d) || TYPE_DOUBLE != (d & 0xf0))
            return false;
        union
        {
#pragma pack(push)
#pragma pack(1)
            struct
            {
                uint8_t t;
                double d;
            } s;
#pragma pack(pop)
            uint8_t b[1 + sizeof(double)] = {};
        } tmp;
        if (!_stream.read(tmp.b, sizeof(tmp.b)))
            return false;
        v = tmp.s.d;
        return true;
    }

    bool rserializer::load(std::string& v)
    {
        uint64_t l;
        if (!load_impl(TYPE_STRING, l, sizeof(uint32_t)))
            return false;
        v.resize(static_cast<size_t>(l));
        return _stream.read(&v[0], static_cast<size_t>(l));
    }

    bool rserializer::load(variant& v)
    {
        uint8_t type;
        if (!peek_type(type))
            return false;
        switch (type)
        {
        case TYPE_BOOL:
            {
                variant::boolean_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_INT8:
            {
                int8_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_UINT8:
            {
                uint8_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_INT16:
            {
                int16_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_UINT16:
            {
                uint16_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_INT32:
            {
                int32_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_UINT32:
            {
                uint32_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_INT64:
            {
                int64_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_POINTER:
        case TYPE_UINT64:
            {
                uint64_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_FLOAT:
            {
                variant::float32_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_DOUBLE:
            {
                variant::float64_t x;
                if (!load(x))
                    return false;
                v.set(x);
                return true;
            }
        case TYPE_BINARY:
        case TYPE_STRING:
            {
                variant::string_t x;
                if (!load(x))
                    return false;
                v.set(std::move(x));
                return true;
            }
        case TYPE_LIST:
            {
                variant::array_t x;
                if (!load(x))
                    return false;
                v.set(std::move(x));
                return true;
            }
        case TYPE_MAP:
            {
                variant::object_t x;
                if (!load(x))
                    return false;
                v.set(std::move(x));
                return true;
            }
        default:
            return false;
        }
    }

    bool rserializer::peek_type(uint8_t& t)
    {
        if (!_stream.peek(t))
            return false;
        t &= 0xf0;
        return true;
    }

    bool rserializer::load_impl(uint8_t t, uint64_t& v, uint8_t l)
    {
        uint8_t d = 0;
        if (!_stream.peek(d) || t != (d & 0xf0))
            return false;

        uint8_t buf[2 + 8] = {};
        uint8_t mask = 0;
        uint8_t masksize = 0;
        if (l <= 4)
        {
            mask = (d & 0xf);
            masksize = 1;
        }
        else
        {
            if (!_stream.peek(buf, 2))
                return false;
            mask = buf[1];
            masksize = 2;
        }

        uint8_t num = 0;
        {
            uint8_t test = 0x1;
            while (test)
            {
                if (mask & test)
                    ++num;
                test <<= 1;
            }
        }

        if (!_stream.read(buf, masksize + num))
            return false;

        v = 0;
        for (uint32_t i = 0, n = 0; i < 8 && n < num; ++i)
        {
            const uint8_t test = (0x1 << i);
            if (0 != (mask & test))
                v |= static_cast<uint64_t>(buf[masksize + n++]) << (i << 3);
        }

        return true;
    }
}
