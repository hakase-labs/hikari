#include "hikari/core/math/NESNumber.hpp"
#include <cmath>

namespace hikari {

    NESNumber::NESNumber()
        : high(0)
        , low(0) 
    {

    }

    NESNumber::NESNumber(const int& value)
        : high(value)
    {

    }

    NESNumber::NESNumber(const char& value)
        : high(value)
    {

    }

    NESNumber::NESNumber(const float& value) {
        NESNumber converted = NESNumber::fromFloat(value);
        setHigh(converted.getHigh());
        setLow(converted.getLow());
    }

    NESNumber::NESNumber(const double& value) {
        NESNumber converted = NESNumber::fromDouble(value);
        setHigh(converted.getHigh());
        setLow(converted.getLow());
    }

    NESNumber::NESNumber(const int& high, const unsigned char& low)
        : high(high)
        , low(low)
    {

    }

    NESNumber::NESNumber(const NESNumber& other)
        : high(other.high)
        , low(other.low)
    {

    }

    NESNumber NESNumber::fromDouble(const double& d) {
        return NESNumber::fromFloat(static_cast<float>(d));
    }

    NESNumber NESNumber::fromFloat(const float& f) {
        int high = static_cast<int>(f);
        float frac = std::abs(f - high);
        unsigned char low = static_cast<unsigned char>(frac * 256.0f);

        return NESNumber(high, low);
    }

    NESNumber NESNumber::fromInt(const int& i) {
        return NESNumber(i, 0);
    }

    const int& NESNumber::getHigh() const {
        return high;
    }

    const unsigned char& NESNumber::getLow() const {
        return low;
    }

    void NESNumber::setHigh(const int& high) {
        this->high = high;
    }

    void NESNumber::setLow(const unsigned char& low) {
        this->low = low;
    }

    double NESNumber::toDouble() const {
        return static_cast<double>(toFloat());
    }

    float NESNumber::toFloat() const {
        float result = static_cast<float>(high);
        float frac = static_cast<float>(low) / 256.0f;

        if(high >= 0) {
            result += frac;
        } else {
            result -= frac;
        }

        return result;
    }

    int NESNumber::toInt() const {
        return getHigh();
    }

    NESNumber NESNumber::operator-() const {
        return NESNumber(-getHigh(), getLow());
    }

    NESNumber& NESNumber::operator += (const NESNumber& rhs) {
        const unsigned int oldLow = getLow();

        high += rhs.getHigh();
        low += rhs.getLow();

        // Check for overflow
        // If there's an overflow we adjust the high accordingly
        if(oldLow > low) {
            // TODO: What if the rhs is negative?
            high++;
        }

        return *this;
    }

    NESNumber& NESNumber::operator -= (const NESNumber& rhs) {
        const unsigned int oldLow = getLow();

        high -= rhs.getHigh();
        low -= rhs.getLow();

        // Check for overflow
        // If there's an overflow we adjust the high accordingly
        if(oldLow < low) {
            high--;

            // TODO: What if the rhs is negative?
            
        }

        return *this;
    }

    NESNumber& NESNumber::operator *= (float k) {
        float result = this->toFloat() * k;
        NESNumber shadow = NESNumber::fromFloat(result);

        setHigh(shadow.getHigh());
        setLow(shadow.getLow());

        return *this;
    }

    NESNumber NESNumber::operator + (const NESNumber& rhs) const {
        NESNumber result(*this);
        result += rhs;

        return result;
    }

    NESNumber NESNumber::operator - (const NESNumber& rhs) const {
        NESNumber result(*this);
        result -= rhs;

        return result;
    }

    NESNumber NESNumber::operator * (float k) const {
        NESNumber result(*this);

        result *= k;

        return result;
    }

    bool NESNumber::operator == (const NESNumber& other) const {
        return (getHigh() == other.getHigh()) && (getLow() == other.getLow());
    }

    bool NESNumber::operator != (const NESNumber& other) const {
        return !(*this == other);
    }

    bool NESNumber::operator < (const NESNumber& other) const {
        // If the high bits are <, then return that.
        // If not, the high bits have to be equal and the low bits < to be <.
        return getHigh() < other.getHigh() || 
            ((getHigh() == other.getHigh()) && (getLow() < other.getLow()));
    }

    bool NESNumber::operator > (const NESNumber& other) const {
        // If the high bits are >, then return that.
        // If not, the high bits have to be equal and the low bits > to be >.
        return getHigh() > other.getHigh() || 
            ((getHigh() == other.getHigh()) && (getLow() > other.getLow()));
    }

    bool NESNumber::operator <= (const NESNumber& other) const {
        return *this < other || *this == other;
    }

    bool NESNumber::operator >= (const NESNumber& other) const {
        return *this > other || *this == other;
    }

}