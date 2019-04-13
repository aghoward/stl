#pragma once

#include <type_traits.h>

namespace std {
    template <typename T>
    constexpr std::remove_reference_t<T>&& move(T&& value)
    {
        return static_cast<std::remove_reference_t<T>&&>(value);
    }

    template <typename T>
    constexpr T&& forward(std::remove_reference_t<T>&& value)
    {
        return static_cast<T&&>(value);
    }

    template <typename T>
    constexpr T&& forward(std::remove_reference_t<T>& value)
    {
        return static_cast<T&&>(value);
    }
}
