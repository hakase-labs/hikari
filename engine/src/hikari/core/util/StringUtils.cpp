#include "hikari/core/util/StringUtils.hpp"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace hikari {
    
    bool StringUtils::endsWith(const std::string &fullString, const std::string &ending) {
        if(fullString.length() > ending.length()) {
            return (0 == fullString.compare(
                fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }

    std::string StringUtils::toLowerCase(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](const char &c){ return std::tolower(c); });
        return s;
    }

    std::string StringUtils::toUpperCase(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](const char &c){ return std::toupper(c); });
        return s;
    }

    // trim from start
    std::string StringUtils::ltrim(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    std::string StringUtils::rtrim(std::string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    std::string StringUtils::trim(std::string s) {
        return StringUtils::ltrim(StringUtils::rtrim(s));
    }

} // hikari