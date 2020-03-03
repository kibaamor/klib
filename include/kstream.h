#pragma once
#include <cstring>
#include <cstdint>
#include <type_traits>
#include <utility>


namespace klib
{
    class rstream
    {
    public:
        virtual ~rstream() = default;

        virtual bool peek(void* data, size_t size) = 0;
        virtual bool discard(size_t size) = 0;
        virtual bool read(void* data, size_t size) = 0;

        template <typename T, typename = typename std::enable_if<std::is_pod<T>::value>::type>
        bool peek(T& data)
        {
            return peek(&data, sizeof(T));
        }

        template <typename T, typename = typename std::enable_if<std::is_pod<T>::value>::type>
        bool read(T& data)
        {
            return read(&data, sizeof(T));
        }
    };

    class wstream
    {
    public:
        virtual ~wstream() = default;

        virtual bool write(const void* data, size_t size) = 0;

        template <typename T, typename = typename std::enable_if<std::is_pod<T>::value>::type>
        bool write(const T data)
        {
            return write(&data, sizeof(T));
        }
    };

    class stream : public rstream, public wstream
    {
    };

    class rbufstream : public rstream
    {
    public:
        rbufstream(void* data, size_t size) noexcept
            : _data(static_cast<uint8_t*>(data)), _size(size), _read(0)
        {
        }
        ~rbufstream() override = default;

        bool peek(void* data, size_t size) noexcept override;
        bool discard(size_t size) noexcept override;
        bool read(void* data, size_t size) noexcept override;

        uint8_t* read_ptr() const noexcept
        {
            return _data + _read;
        }
        void add_read(size_t size) noexcept
        {
            _read += size; if (_read > _size) _read = size;
        }
        size_t read_size() const noexcept
        {
            return _size - _read;
        }

    private:
        uint8_t* const _data;
        const size_t _size;
        size_t _read;
    };

    class wbufstream : public wstream
    {
    public:
        wbufstream(void* data, size_t size) noexcept
            : _data(static_cast<uint8_t*>(data)), _size(size), _write(0)
        {
        }
        ~wbufstream() override = default;

        bool write(const void* data, size_t size) noexcept override;

        uint8_t* write_ptr() const noexcept
        {
            return _data + _write;
        }
        void add_write(size_t size) noexcept
        {
            _write += size; if (_write > _size) _write = size;
        }
        size_t write_size() const noexcept
        {
            return _size - _write;
        }

    private:
        uint8_t* const _data;
        const size_t _size;
        size_t _write;
    };

    class bufstream : public rbufstream, public wbufstream
    {
    public:
        bufstream(void* data, size_t size) noexcept
            : rbufstream(data, size), wbufstream(data, size)
        {
        }
        ~bufstream() override = default;
    };

    class memstream : public stream
    {
    public:
        memstream() noexcept = default;
        memstream(const void* data, size_t size);
        memstream(const memstream& other);
        memstream(memstream&& other) noexcept;
        memstream& operator=(const memstream& other);
        memstream& operator=(memstream&& other) noexcept;
        ~memstream() override;

        void clear() noexcept;
        void reset_with_copy(const void* data, size_t size);
        void reset_with_own(void* data, size_t size) noexcept;

        void swap(memstream& s);

        bool peek(void* data, size_t size) noexcept override;
        bool discard(size_t size) noexcept override;
        bool read(void* data, size_t size) noexcept override;

        uint8_t* read_ptr() const noexcept
        {
            return _data + _read;
        }
        void add_read(size_t size) noexcept
        {
            _read += size; if (_read > _write) _read = _write;
        }
        size_t read_size() const noexcept
        {
            return _write - _read;
        }

        bool write(const void* data, size_t size) override;

        uint8_t* write_ptr() const noexcept
        {
            return _data + _write;
        }
        void add_write(size_t size) noexcept
        {
            _write += size; if (_write > _size) _write = size;
        }
        void ensure_write(size_t size);

    private:
        uint8_t* _data = nullptr;
        size_t _size = 0;
        size_t _read = 0;
        size_t _write = 0;
    };
}

namespace std
{
    inline void swap(klib::memstream& a, klib::memstream& b)
    {
        a.swap(b);
    }
}
