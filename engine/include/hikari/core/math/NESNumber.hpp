#ifndef HIKARI_CORE_MATH_NESNUMBER
#define HIKARI_CORE_MATH_NESNUMBER

namespace hikari {

class NESNumber {
private:
    int high;
    unsigned char low;
    
public:
    NESNumber();
    NESNumber(const int& value);
    NESNumber(const char& value);
    NESNumber(const float& value);
    NESNumber(const double& value);
    NESNumber(const int& high, const unsigned char& low);
    NESNumber(const NESNumber& other);
    
    static NESNumber fromDouble(const double& d);
    static NESNumber fromFloat(const float& f);
    static NESNumber fromInt(const int& i);
    
    const int& getHigh() const;
    const unsigned char& getLow() const;

    void setHigh(const int& high);
    void setLow(const unsigned char& low);
    
    double toDouble() const;
    float toFloat() const;
    int toInt() const;

    NESNumber operator-() const;

    NESNumber& operator += (const NESNumber& rhs);
    NESNumber& operator -= (const NESNumber& rhs);
    NESNumber& operator *= (const NESNumber& rhs);
    NESNumber& operator *= (float k);
    //NESNumber& operator /= (const NESNumber& rhs);
    //NESNumber& operator /= (float k);

    NESNumber operator + (const NESNumber& rhs) const;
    NESNumber operator - (const NESNumber& rhs) const;
    NESNumber operator * (const NESNumber& rhs) const;
    NESNumber operator * (float k) const;
    //NESNumber operator / (const NESNumber& rhs) const;
    //NESNumber operator / (float k) const;

    bool operator == (const NESNumber& other) const;
    bool operator != (const NESNumber& other) const;
    bool operator < (const NESNumber& other) const;
    bool operator > (const NESNumber& other) const;
    bool operator <= (const NESNumber& other) const;
    bool operator >= (const NESNumber& other) const;
};

} // hikari

#endif // HIKARI_CORE_MATH_NESNumber