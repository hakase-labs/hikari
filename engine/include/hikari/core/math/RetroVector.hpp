#ifndef HIKARI_CORE_MATH_RETROVECTOR
#define HIKARI_CORE_MATH_RETROVECTOR

namespace hikari {

class RetroVector {
private:
    int high;
    unsigned char low;
    
public:
    RetroVector();
    RetroVector(const int& value);
    RetroVector(const char& value);
    RetroVector(const float& value);
    RetroVector(const double& value);
    RetroVector(const int& high, const unsigned char& low);
    RetroVector(const RetroVector& other);
    
    static RetroVector fromDouble(const double& d);
    static RetroVector fromFloat(const float& f);
    static RetroVector fromInt(const int& i);
    
    const int& getHigh() const;
    const unsigned char& getLow() const;

    void setHigh(const int& high);
    void setLow(const unsigned char& low);
    
    double toDouble() const;
    float toFloat() const;
    int toInt() const;

    RetroVector operator-() const;

    RetroVector& operator += (const RetroVector& rhs);
    RetroVector& operator -= (const RetroVector& rhs);
    RetroVector& operator *= (const RetroVector& rhs);
    RetroVector& operator *= (float k);
    //RetroVector& operator /= (const RetroVector& rhs);
    //RetroVector& operator /= (float k);

    RetroVector operator + (const RetroVector& rhs) const;
    RetroVector operator - (const RetroVector& rhs) const;
    RetroVector operator * (const RetroVector& rhs) const;
    RetroVector operator * (float k) const;
    //RetroVector operator / (const RetroVector& rhs) const;
    //RetroVector operator / (float k) const;

    bool operator == (const RetroVector& other) const;
    bool operator != (const RetroVector& other) const;
    bool operator < (const RetroVector& other) const;
    bool operator > (const RetroVector& other) const;
    bool operator <= (const RetroVector& other) const;
    bool operator >= (const RetroVector& other) const;
};

} // hikari

#endif // HIKARI_CORE_MATH_RETROVECTOR