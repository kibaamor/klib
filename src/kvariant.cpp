#include "../include/kvariant.h"
#include <cassert>

namespace klib {

bool is_integral_vtype(vtype_t t) noexcept
{
    switch (t) {
    case vtype_t::boolean:
    case vtype_t::int8:
    case vtype_t::uint8:
    case vtype_t::int16:
    case vtype_t::uint16:
    case vtype_t::int32:
    case vtype_t::uint32:
    case vtype_t::int64:
    case vtype_t::uint64:
        return true;
    case vtype_t::null:
    case vtype_t::float32:
    case vtype_t::float64:
    case vtype_t::string:
    case vtype_t::array:
    case vtype_t::object:
    default:
        return false;
    }
}

bool is_floatpoint_vtype(vtype_t t) noexcept
{
    switch (t) {
    case vtype_t::float32:
    case vtype_t::float64:
        return true;
    case vtype_t::null:
    case vtype_t::boolean:
    case vtype_t::int8:
    case vtype_t::uint8:
    case vtype_t::int16:
    case vtype_t::uint16:
    case vtype_t::int32:
    case vtype_t::uint32:
    case vtype_t::int64:
    case vtype_t::uint64:
    case vtype_t::string:
    case vtype_t::array:
    case vtype_t::object:
    default:
        return false;
    }
}

const char* get_vtype_name(vtype_t t) noexcept
{
    switch (t) {
    case vtype_t::null:
        return "null";
    case vtype_t::boolean:
        return "boolean";
    case vtype_t::int8:
        return "int8";
    case vtype_t::uint8:
        return "uint8";
    case vtype_t::int16:
        return "int16";
    case vtype_t::uint16:
        return "uint16";
    case vtype_t::int32:
        return "int32";
    case vtype_t::uint32:
        return "uint32";
    case vtype_t::int64:
        return "int64";
    case vtype_t::uint64:
        return "uint64";
    case vtype_t::float32:
        return "float32";
    case vtype_t::float64:
        return "float64";
    case vtype_t::string:
        return "string";
    case vtype_t::array:
        return "array";
    case vtype_t::object:
        return "object";
    default:
        return "";
    }
}

variant::variant(const variant& other)
{
    _type = other._type;
    switch (_type) {
    case vtype_t::string:
        _value.str = new string_t(*other._value.str);
        break;
    case vtype_t::array:
        _value.arr = new array_t(*other._value.arr);
        break;
    case vtype_t::object:
        _value.obj = new object_t(*other._value.obj);
        break;
    default:
        _value.u64 = other._value.u64;
        break;
    }
}

variant& variant::operator=(const variant& other)
{
    if (this != &other) {
        clear();
        _type = other._type;
        switch (_type) {
        case vtype_t::string:
            _value.str = new string_t(*other._value.str);
            break;
        case vtype_t::array:
            _value.arr = new array_t(*other._value.arr);
            break;
        case vtype_t::object:
            _value.obj = new object_t(*other._value.obj);
            break;
        default:
            _value.u64 = other._value.u64;
            break;
        }
    }
    return *this;
}
variant::variant(variant&& other) noexcept
{
    _type = other._type;
    _value = other._value;
    other._type = vtype_t::null;
    other._value = {};
}

variant& variant::operator=(variant&& other) noexcept
{
    if (this != &other) {
        clear();
        _type = other._type;
        _value = other._value;
        other._type = vtype_t::null;
        other._value = {};
    }
    return *this;
}

void variant::clear() noexcept
{
    assert_valid();
    switch (_type) {
    case vtype_t::string:
        delete _value.str;
        break;
    case vtype_t::array:
        delete _value.arr;
        break;
    case vtype_t::object:
        delete _value.obj;
        break;
    default:
        break;
    }
    _type = vtype_t::null;
    _value = {};
}

void variant::swap(variant& other) noexcept
{
    assert_valid();
    other.assert_valid();
    using std::swap;
    swap(this->_type, other._type);
    swap(this->_value, other._value);
}

std::string variant::to_string() const
{
    std::string str = std::string("variant(") + get_vtype_name(_type) + ", ";
    switch (_type) {
    case vtype_t::null:
        str += "null";
        break;
    case vtype_t::boolean:
        str += (this->get<boolean_t>() ? "true" : "false");
        break;
    case vtype_t::int8:
        str += std::to_string(this->get<int8_t>());
        break;
    case vtype_t::uint8:
        str += std::to_string(this->get<uint8_t>());
        break;
    case vtype_t::int16:
        str += std::to_string(this->get<int16_t>());
        break;
    case vtype_t::uint16:
        str += std::to_string(this->get<uint16_t>());
        break;
    case vtype_t::int32:
        str += std::to_string(this->get<int32_t>());
        break;
    case vtype_t::uint32:
        str += std::to_string(this->get<uint32_t>());
        break;
    case vtype_t::int64:
        str += std::to_string(this->get<int64_t>());
        break;
    case vtype_t::uint64:
        str += std::to_string(this->get<uint64_t>());
        break;
    case vtype_t::float32:
        str += std::to_string(this->get<float32_t>());
        break;
    case vtype_t::float64:
        str += std::to_string(this->get<float64_t>());
        break;
    case vtype_t::string:
        str += std::string("size:") + std::to_string(this->get<std::string>().size());
        break;
    case vtype_t::array: {
        const array_t& arr = this->get<array_t>();
        str += "{";
        bool flag = false;
        for (const variant& v : arr) {
            if (flag)
                str += ", " + v.to_string();
            else
                str += v.to_string();
            flag = true;
        }
        str += "}";
    } break;
    case vtype_t::object: {
        const object_t& obj = this->get<object_t>();
        str += "{";
        bool flag = false;
        for (const auto& kv : obj) {
            if (flag)
                str += ", " + kv.first.to_string() + ": " + kv.second.to_string();
            else
                str += kv.first.to_string() + ": " + kv.second.to_string();
            flag = true;
        }
        str += "}";
    } break;
    default:
        break;
    }
    str += ")";
    return str;
}

void variant::assert_valid() const
{
    assert(vtype_t::string != _type || nullptr != _value.str);
    assert(vtype_t::array != _type || nullptr != _value.arr);
    assert(vtype_t::object != _type || nullptr != _value.obj);
}

int64_t variant::get_integral_value() const noexcept
{
    switch (_type) {
    case vtype_t::null:
        return 0;
    case vtype_t::boolean:
        return static_cast<int64_t>(_value.b ? 1 : 0);
    case vtype_t::int8:
        return static_cast<int64_t>(_value.i8);
    case vtype_t::uint8:
        return static_cast<int64_t>(_value.u8);
    case vtype_t::int16:
        return static_cast<int64_t>(_value.i16);
    case vtype_t::uint16:
        return static_cast<int64_t>(_value.u16);
    case vtype_t::int32:
        return static_cast<int64_t>(_value.i32);
    case vtype_t::uint32:
        return static_cast<int64_t>(_value.u32);
    case vtype_t::int64:
        return static_cast<int64_t>(_value.i64);
    case vtype_t::uint64:
        return static_cast<int64_t>(_value.u64);
    case vtype_t::float32:
        return static_cast<int64_t>(_value.f32);
    case vtype_t::float64:
        return static_cast<int64_t>(_value.f64);
    case vtype_t::string:
        return 0;
    case vtype_t::array:
        return 0;
    case vtype_t::object:
        return 0;
    default:
        return 0;
    }
}

variant::float64_t variant::get_floatpoint_value() const noexcept
{
    switch (_type) {
    case vtype_t::null:
        return 0.0;
    case vtype_t::boolean:
        return static_cast<float64_t>(_value.b ? 1 : 0);
    case vtype_t::int8:
        return static_cast<float64_t>(_value.i8);
    case vtype_t::uint8:
        return static_cast<float64_t>(_value.u8);
    case vtype_t::int16:
        return static_cast<float64_t>(_value.i16);
    case vtype_t::uint16:
        return static_cast<float64_t>(_value.u16);
    case vtype_t::int32:
        return static_cast<float64_t>(_value.i32);
    case vtype_t::uint32:
        return static_cast<float64_t>(_value.u32);
    case vtype_t::int64:
        return static_cast<float64_t>(_value.i64);
    case vtype_t::uint64:
        return static_cast<float64_t>(_value.u64);
    case vtype_t::float32:
        return static_cast<float64_t>(_value.f32);
    case vtype_t::float64:
        return static_cast<float64_t>(_value.f64);
    case vtype_t::string:
        return 0.0;
    case vtype_t::array:
        return 0.0;
    case vtype_t::object:
        return 0.0;
    default:
        return 0;
    }
}

bool operator==(const variant& lhs, const variant& rhs) noexcept
{
    const vtype_t lhs_type = lhs._type;
    const vtype_t rhs_type = rhs._type;
    if (lhs_type == rhs_type) {
        switch (lhs_type) {
        case vtype_t::null:
            return true;
        case vtype_t::boolean:
            return lhs._value.b == rhs._value.b;
        case vtype_t::int8:
            return lhs._value.i8 == rhs._value.i8;
        case vtype_t::uint8:
            return lhs._value.u8 == rhs._value.u8;
        case vtype_t::int16:
            return lhs._value.i16 == rhs._value.i16;
        case vtype_t::uint16:
            return lhs._value.u16 == rhs._value.u16;
        case vtype_t::int32:
            return lhs._value.i32 == rhs._value.i32;
        case vtype_t::uint32:
            return lhs._value.u32 == rhs._value.u32;
        case vtype_t::int64:
            return lhs._value.i64 == rhs._value.i64;
        case vtype_t::uint64:
            return lhs._value.u64 == rhs._value.u64;
        case vtype_t::float32:
            return lhs._value.f32 == rhs._value.f32;
        case vtype_t::float64:
            return lhs._value.f64 == rhs._value.f64;
        case vtype_t::string:
            return (*lhs._value.str) == (*rhs._value.str);
        case vtype_t::array:
            return (*lhs._value.arr) == (*rhs._value.arr);
        case vtype_t::object:
            return (*lhs._value.obj) == (*rhs._value.obj);
        default:
            return false;
        }
    } else if (is_integral_vtype(lhs_type) && is_integral_vtype(rhs_type)) {
        return lhs.get_integral_value() == rhs.get_integral_value();
    } else if ((is_floatpoint_vtype(lhs_type) && is_integral_vtype(rhs_type))
        || (is_integral_vtype(lhs_type) && is_floatpoint_vtype(rhs_type))) {
        return lhs.get_floatpoint_value() == rhs.get_floatpoint_value();
    }
    return false;
}

bool operator<(const variant& lhs, const variant& rhs) noexcept
{
    const vtype_t lhs_type = lhs._type;
    const vtype_t rhs_type = rhs._type;
    if (lhs_type == rhs_type) {
        switch (lhs_type) {
        case vtype_t::null:
            return true;
        case vtype_t::boolean:
            return lhs._value.b < rhs._value.b;
        case vtype_t::int8:
            return lhs._value.i8 < rhs._value.i8;
        case vtype_t::uint8:
            return lhs._value.u8 < rhs._value.u8;
        case vtype_t::int16:
            return lhs._value.i16 < rhs._value.i16;
        case vtype_t::uint16:
            return lhs._value.u16 < rhs._value.u16;
        case vtype_t::int32:
            return lhs._value.i32 < rhs._value.i32;
        case vtype_t::uint32:
            return lhs._value.u32 < rhs._value.u32;
        case vtype_t::int64:
            return lhs._value.i64 < rhs._value.i64;
        case vtype_t::uint64:
            return lhs._value.u64 < rhs._value.u64;
        case vtype_t::float32:
            return lhs._value.f32 < rhs._value.f32;
        case vtype_t::float64:
            return lhs._value.f64 < rhs._value.f64;
        case vtype_t::string:
            return (*lhs._value.str) < (*rhs._value.str);
        case vtype_t::array:
            return (*lhs._value.arr) < (*rhs._value.arr);
        case vtype_t::object:
            return (*lhs._value.obj) < (*rhs._value.obj);
        default:
            return false;
        }
    } else if (is_integral_vtype(lhs_type) && is_integral_vtype(rhs_type)) {
        return lhs.get_integral_value() < rhs.get_integral_value();
    } else if ((is_floatpoint_vtype(lhs_type) && is_integral_vtype(rhs_type))
        || (is_integral_vtype(lhs_type) && is_floatpoint_vtype(rhs_type))) {
        return lhs.get_floatpoint_value() < rhs.get_floatpoint_value();
    }
    return operator<(lhs, rhs);
}

bool from_variant(const variant& var, variant::boolean_t& v)
{
    if (vtype_t::boolean != var.get_type())
        return false;
    v = var.get<variant::boolean_t>();
    return true;
}
bool from_variant(const variant& var, int8_t& v)
{
    if (vtype_t::int8 != var.get_type())
        return false;
    v = var.get<int8_t>();
    return true;
}
bool from_variant(const variant& var, uint8_t& v)
{
    if (vtype_t::uint8 != var.get_type())
        return false;
    v = var.get<uint8_t>();
    return true;
}
bool from_variant(const variant& var, int16_t& v)
{
    if (vtype_t::int16 != var.get_type())
        return false;
    v = var.get<int16_t>();
    return true;
}
bool from_variant(const variant& var, uint16_t& v)
{
    if (vtype_t::uint16 != var.get_type())
        return false;
    v = var.get<uint16_t>();
    return true;
}
bool from_variant(const variant& var, int32_t& v)
{
    if (vtype_t::int32 != var.get_type())
        return false;
    v = var.get<int32_t>();
    return true;
}
bool from_variant(const variant& var, uint32_t& v)
{
    if (vtype_t::uint32 != var.get_type())
        return false;
    v = var.get<uint32_t>();
    return true;
}
bool from_variant(const variant& var, int64_t& v)
{
    if (vtype_t::int64 != var.get_type())
        return false;
    v = var.get<int64_t>();
    return true;
}
bool from_variant(const variant& var, uint64_t& v)
{
    if (vtype_t::uint64 != var.get_type())
        return false;
    v = var.get<uint64_t>();
    return true;
}
bool from_variant(const variant& var, variant::float32_t& v)
{
    if (vtype_t::float32 != var.get_type())
        return false;
    v = var.get<variant::float32_t>();
    return true;
}
bool from_variant(const variant& var, variant::float64_t& v)
{
    if (vtype_t::float64 != var.get_type())
        return false;
    v = var.get<variant::float64_t>();
    return true;
}
bool from_variant(const variant& var, variant::string_t& v)
{
    if (vtype_t::string != var.get_type())
        return false;
    v = var.get<variant::string_t>();
    return true;
}
bool from_variant(const variant& var, variant::array_t& v)
{
    if (vtype_t::array != var.get_type())
        return false;
    v = var.get<variant::array_t>();
    return true;
}
bool from_variant(const variant& var, variant::object_t& v)
{
    if (vtype_t::object != var.get_type())
        return false;
    v = var.get<variant::object_t>();
    return true;
}

} // namespace klib
