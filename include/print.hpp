#pragma once

#include <iostream>
#include <type_traits>

namespace std
{
    // overload for fundamental types
    // template<typename C, class = typename std::enable_if<std::is_fundamental<C>::value || std::is_class<C>::value>::type>
    template<typename C, class = typename std::enable_if<std::is_fundamental<C>::value>::type>
    void print(C var)
    {
        std::cout << var;
    }

    // overload for container types
    template<typename C, typename T = typename C::value_type>
    void print(C const& var)
    {
        std::cout << "{";
        for(T x : var)
        {
            print(x);
            std::cout << ", ";
        }
        std::cout << "}";
    }

    // overload for string
    void print(std::string const& var)
    {
        std::cout << var;
    }

    template<typename C>
    void println(C const& var)
    {
        print(var);
        std::cout << std::endl;
    }
}

