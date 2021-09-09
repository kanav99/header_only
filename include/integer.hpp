#pragma once

#include <type_traits>
#include <ostream>

template <int B>
using suitable_uint = std::conditional_t<B <= 8, uint8_t,
                                         std::conditional_t<B <= 16, uint16_t,
                                                            std::conditional_t<B <= 32, uint32_t, uint64_t>>>;

// type trait to see if B bits of data can fit in the type
template <int B, typename T>
struct can_fit
{
    static const bool value = (sizeof(T) * 8 >= B);
};


// Policy:
//   { +, - } -> use operand with max bitsize
//   { * } -> add bitsizes
//   { = } -> truncate if needed

template <int B, typename T = suitable_uint<B>,
          class = typename std::enable_if<can_fit<B, T>::value>::type>
class uinteger
{
public:
    typedef T int_type;
    static const T MAX = 1 << B;
    T value;

    uinteger(T value) : value(value % MAX)
    {
    }

    uinteger() : value(0)
    {
    }

    template <int B2, typename T2>
    uinteger(uinteger<B2, T2> x) : value(x.value % MAX)
    {
    }

    inline int bitsize() const { return B; }

    uinteger<B, T> operator+(T v)
    {
        return uinteger<B, T>((value + v) % MAX);
    }

    template <int B2, typename T2, class = typename std::enable_if<std::greater<int>()(B2, B)>::type>
    uinteger<B2, T2> operator+(uinteger<B2, T2> v)
    {
        return uinteger<B2, T2>((value + v.value) % uinteger<B2, T2>::MAX);
    }

    template <int B2, typename T2, class = typename std::enable_if<std::less_equal<int>()(B2, B)>::type>
    uinteger<B, T> operator+(uinteger<B2, T2> v)
    {
        return uinteger<B, T>((value + v.value) % MAX);
    }

    uinteger<B, T> operator-(T v)
    {
        return uinteger<B, T>((value - v) % MAX);
    }

    template <int B2, typename T2, class = typename std::enable_if<std::greater<int>()(B2, B)>::type>
    uinteger<B2, T2> operator-(uinteger<B2, T2> v)
    {
        return uinteger<B2, T2>((value - v.value) % uinteger<B2, T2>::MAX);
    }

    template <int B2, typename T2, class = typename std::enable_if<std::less_equal<int>()(B2, B)>::type>
    uinteger<B, T> operator-(uinteger<B2, T2> v)
    {
        return uinteger<B, T>((value - v.value) % MAX);
    }

    template <int B2, typename T2>
    uinteger<B + B2> operator*(uinteger<B2, T2> v)
    {
        auto n = uinteger<B + B2>(value);
        n.value *= v.value;
        return n;
    }

    template <typename T2, class = typename std::enable_if<std::is_integral<T2>::value>::type>
    uinteger<B + sizeof(T2)> operator*(T2 v)
    {
        auto n = uinteger<B + sizeof(T2)>(value);
        n.value *= v;
        return n;
    }

    explicit operator int() const
    {
        return value;
    }

    template <int s>
    uinteger<B - s> rightShift()
    {
        return uinteger<B - s>(value >> s);
    }

    uint8_t const &operator[](int index) const
    {
        assert(index < B);
        return static_cast<uint8_t>(value >> (B - 1 - index)) & 1;
    }

    uinteger<B> operator-()
    {
        uinteger<B> res = (1 << B) - value;
        return res;
    }
};

template <int B, typename T = suitable_uint<B>>
std::ostream &operator<<(std::ostream &os, uinteger<B, T> const &m)
{
    return os << m.value;
}

template <int B, class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
uinteger<B> operator+(T l, uinteger<B> r)
{
    uinteger<B> res = r;
    res.value = (res.value + l) % (1 << B);
    return res;
}

template <int B, class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
uinteger<B> operator-(T l, uinteger<B> r)
{
    uinteger<B> res = r;
    res.value = ((suitable_uint<B>)(l) - res.value) % (1 << B);
    return res;
}
