#include "../include/kstream.h"

namespace klib {

bool rbufstream::peek(void* data, size_t size) noexcept
{
    if (0 == size)
        return true;
    if (read_size() < size)
        return false;
    std::memcpy(data, read_ptr(), size);
    return true;
}

bool rbufstream::discard(size_t size) noexcept
{
    if (read_size() < size)
        return false;
    add_read(size);
    return true;
}

bool rbufstream::read(void* data, size_t size) noexcept
{
    if (0 == size)
        return true;
    if (read_size() < size)
        return false;
    std::memcpy(data, read_ptr(), size);
    add_read(size);
    return true;
}

bool wbufstream::write(const void* data, size_t size) noexcept
{
    if (0 == size)
        return true;
    if (write_size() < size)
        return false;
    std::memcpy(write_ptr(), data, size);
    add_write(size);
    return true;
}

memstream::memstream(const void* data, size_t size)
{
    reset_with_copy(data, size);
}

memstream::memstream(const memstream& other)
{
    *this = other;
}

memstream::memstream(memstream&& other) noexcept
{
    *this = std::move(other);
}

memstream& memstream::operator=(const memstream& other)
{
    if (this != &other) {
        reset_with_copy(other._data, other._size);
        this->_read = other._read;
        this->_write = other._write;
    }
    return *this;
}

memstream& memstream::operator=(memstream&& other) noexcept
{
    if (this != &other) {
        reset_with_own(other._data, other._size);
        this->_read = other._read;
        this->_write = other._write;

        other._data = nullptr;
        other._size = other._read = other._write = 0;
    }
    return *this;
}

memstream::~memstream()
{
    clear();
}

void memstream::clear() noexcept
{
    if (nullptr != _data) {
        delete[] _data;
        _data = nullptr;
        _size = _read = _write = 0;
    }
}

void memstream::reset_with_copy(const void* data, size_t size)
{
    clear();
    write(data, size);
}

void memstream::reset_with_own(void* data, size_t size) noexcept
{
    clear();
    _data = static_cast<uint8_t*>(data);
    _size = _write = size;
}

void memstream::swap(memstream& s)
{
    using std::swap;
    swap(_data, s._data);
    swap(_size, s._size);
    swap(_read, s._read);
    swap(_write, s._write);
}

bool memstream::peek(void* data, size_t size) noexcept
{
    if (0 == size)
        return true;
    if (read_size() < size)
        return false;
    std::memcpy(data, read_ptr(), size);
    return true;
}

bool memstream::discard(size_t size) noexcept
{
    if (read_size() < size)
        return false;
    add_read(size);
    return true;
}

bool memstream::read(void* data, size_t size) noexcept
{
    if (0 == size)
        return true;
    if (read_size() < size)
        return false;
    std::memcpy(data, read_ptr(), size);
    add_read(size);
    return true;
}

bool memstream::write(const void* data, size_t size)
{
    if (0 == size)
        return true;
    ensure_write(size);
    std::memcpy(write_ptr(), data, size);
    add_write(size);
    return true;
}

void memstream::ensure_write(size_t size)
{
    if (_write + size <= _size)
        return;

    const uint8_t* readptr = read_ptr();
    const size_t readsize = read_size();

    if (_read >= size) {
        std::memmove(_data, readptr, readsize);
        _write -= _read;
        _read = 0;
        return;
    }

    size_t newsize = _size + _size / 2; // 150%
    if (newsize < _size + size)
        newsize = _size + size;

    uint8_t* newdata = new uint8_t[newsize];
    if (readsize > 0)
        std::memcpy(newdata, readptr, readsize);
    _write -= _read;
    _read = 0;

    delete[] _data;
    _data = newdata;
    _size = newsize;
}

} // namespace klib
