#ifndef HIKARI_CORE_UTIL_STRINGUTILS
#define HIKARI_CORE_UTIL_STRINGUTILS

#include <hikari/core/Platform.hpp>
#include <string>
#include <sstream>
#include <stdexcept>

namespace hikari {

    class HIKARI_API StringUtils {
    public:
        static bool endsWith(const std::string &fullString, const std::string &ending);
        static std::string toLowerCase(std::string s);
        static std::string toUpperCase(std::string s);

        template<typename T>
        static inline std::string toString(const T& t) {
            std::stringstream ss;
            if(!(ss << t)) {
                throw std::runtime_error("Bad conversion to string.");
            }
            return ss.str();
        }

        template<typename T>
        static inline T fromString(const std::string &str) {
            std::stringstream i(str);
            T t;
            if(!(i >> t)) {
                throw std::runtime_error("Bad conversion from string: " + str);
            }
            return t;
        }

        template<>
        static inline float fromString(const std::string &str) {
            return std::stof(str);
        }

        template<>
        static inline double fromString(const std::string &str) {
            return std::stod(str);
        }


        // trim from start
        static inline std::string ltrim(std::string s);

        // trim from end
        static inline std::string rtrim(std::string s);

        // trim from both ends
        static std::string trim(std::string s);
    };

} // hikari

#endif // HIKARI_CORE_UTIL_STRINGUTILS