#include <hikari/core/math/RetroVector.hpp>
#include <cmath>

namespace hikari {

    RetroVector::RetroVector()
        : high(0)
        , low(0) 
    {

    }

    RetroVector::RetroVector(const int& value)
        : high(value)
    {

    }

    RetroVector::RetroVector(const char& value)
        : high(value)
    {

    }

    RetroVector::RetroVector(const float& value) {
        RetroVector converted = RetroVector::fromFloat(value);
        setHigh(converted.getHigh());
        setLow(converted.getLow());
    }

    RetroVector::RetroVector(const double& value) {
        RetroVector converted = RetroVector::fromDouble(value);
        setHigh(converted.getHigh());
        setLow(converted.getLow());
    }

    RetroVector::RetroVector(const int& high, const unsigned char& low)
        : high(high)
        , low(low)
    {

    }

    RetroVector::RetroVector(const RetroVector& other)
        : high(other.high)
        , low(other.low)
    {

    }

    RetroVector RetroVector::fromDouble(const double& d) {
        return RetroVector::fromFloat(static_cast<float>(d));
    }

    RetroVector RetroVector::fromFloat(const float& f) {
        int high = static_cast<int>(f);
        float frac = std::abs(f - high);
        unsigned char low = static_cast<unsigned char>(frac * 256.0f);

        return RetroVector(high, low);
    }

    RetroVector RetroVector::fromInt(const int& i) {
        return RetroVector(i, 0);
    }

    const int& RetroVector::getHigh() const {
        return high;
    }

    const unsigned char& RetroVector::getLow() const {
        return low;
    }

    void RetroVector::setHigh(const int& high) {
        this->high = high;
    }

    void RetroVector::setLow(const unsigned char& low) {
        this->low = low;
    }

    double RetroVector::toDouble() const {
        return static_cast<double>(toFloat());
    }

    float RetroVector::toFloat() const {
        float result = static_cast<float>(high);
        float frac = static_cast<float>(low) / 256.0f;

        if(high >= 0) {
            result += frac;
        } else {
            result -= frac;
        }

        return result;
    }

    int RetroVector::toInt() const {
        return getHigh();
    }

    RetroVector RetroVector::operator-() const {
        return RetroVector(-getHigh(), getLow());
    }

    RetroVector& RetroVector::operator += (const RetroVector& rhs) {
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

    RetroVector& RetroVector::operator -= (const RetroVector& rhs) {
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

    RetroVector& RetroVector::operator *= (float k) {
        float result = this->toFloat() * k;
        RetroVector shadow = RetroVector::fromFloat(result);

        setHigh(shadow.getHigh());
        setLow(shadow.getLow());

        return *this;
    }

    RetroVector RetroVector::operator + (const RetroVector& rhs) const {
        RetroVector result(*this);
        result += rhs;

        return result;
    }

    RetroVector RetroVector::operator - (const RetroVector& rhs) const {
        RetroVector result(*this);
        result -= rhs;

        return result;
    }

    RetroVector RetroVector::operator * (float k) const {
        RetroVector result(*this);

        result *= k;

        return result;
    }

    bool RetroVector::operator == (const RetroVector& other) const {
        return (getHigh() == other.getHigh()) && (getLow() == other.getLow());
    }

    bool RetroVector::operator != (const RetroVector& other) const {
        return !(*this == other);
    }

    bool RetroVector::operator < (const RetroVector& other) const {
        // If the high bits are <, then return that.
        // If not, the high bits have to be equal and the low bits < to be <.
        return getHigh() < other.getHigh() || 
            ((getHigh() == other.getHigh()) && (getLow() < other.getLow()));
    }

    bool RetroVector::operator > (const RetroVector& other) const {
        // If the high bits are >, then return that.
        // If not, the high bits have to be equal and the low bits > to be >.
        return getHigh() > other.getHigh() || 
            ((getHigh() == other.getHigh()) && (getLow() > other.getLow()));
    }

    bool RetroVector::operator <= (const RetroVector& other) const {
        return *this < other || *this == other;
    }

    bool RetroVector::operator >= (const RetroVector& other) const {
        return *this > other || *this == other;
    }

}