#ifndef HIKARI_CORE_MATH_HPP
#define HIKARI_CORE_MATH_HPP

#include <cstdlib>

namespace hikari {
namespace math {

    /**
        Returns the sign of a number if it is non-zero, otherwise returns 0.

        @return 1 if positive, -1 if negative, 0 if 0
    */
    template <class T>
    inline T sign(const T& number) {
        return (number == 0) ? 0 : (number < 0) ? -1 : 1;
    }

    template<>
    inline float sign(const float& number) {
        return (number == 0.0f) ? 0.0f : (number < 0.0f) ? -1.0f : 1.0f;
    }

    /**
        Returns the sign bit of a number.

        @return 0 if number is positive, 1 if number is negative.
    */
    template <class T>
    inline int signbit(const T& number) {
        return (number < 0) ? 1 : 0;
    }

    /**
        TODO: Document this function.
    */
    template <class T>
    inline T copysign(const T& number, const T& signSource) {
        return std::abs(number) * hikari::math::sign(signSource);
    }

    /**
     * @brief Returns the value "value" brought into the range [ lo, hi ].
     * 
     * If the value is greater than "hi", return hi. If the value is
     * less than "lo", return lo. Otherwise, return the original value.
     *
     * @param value   The value to be clamped
     * @param low     The low point of the range to be clamped to
     * @param high    The high point of the range to be clamped to
     */
    template <typename T>
    T clamp(const T& value, const T& low, const T& high) {
        // Alternatively:
        // return std::max(std::min(value, high), low);
        return value < low ? low : (value > high ? high : value);
    }

}
}

#endif // HIKARI_CORE_MATH_HPP
