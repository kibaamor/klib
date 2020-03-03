#pragma once


namespace klib
{

    template <typename S, typename V>
    inline bool serialize_adl(S& s, V& v);

    template <typename A>
    class rserialization
    {
    public:
        A* get_this()
        {
            return static_cast<A*>(this);
        }
        template <typename V>
        bool operator>>(V& v)
        {
            return this->get_this()->load(v);
        }

        template <typename V>
        bool operator&(V& v)
        {
            return this->get_this()->load(v);
        }

        template <typename V>
        bool load(V& v)
        {
            return serialize_adl(*this->get_this(), v);
        }
    };

    template <typename A>
    class wserialization
    {
    public:
        A* get_this()
        {
            return static_cast<A*>(this);
        }

        template <typename V>
        bool operator<<(const V& v)
        {
            return this->get_this()->save(v);
        }

        template <typename V>
        bool operator&(const V& v)
        {
            return this->get_this()->save(v);
        }

        template <typename V>
        bool save(const V& v)
        {
            return serialize_adl(*this->get_this(), const_cast<V &>(v));
        }
    };

    class serialization_access
    {
    public:
        template <typename S, typename V>
        static bool serialize(S& s, V& v)
        {
            return v.serialize(s);
        }

        template <typename S, typename V>
        friend inline bool serialize(S& s, V& v);
    };

    template <typename S, typename V>
    inline bool serialize(S& s, V& v)
    {
        return serialization_access::serialize(s, v);
    }

    template <typename S, typename V>
    inline bool serialize_adl(S& s, V& v)
    {
        return serialize(s, v);
    }
}
