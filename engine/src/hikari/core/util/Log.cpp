#include "hikari/core/util/Log.hpp"
#include <ctime>
#include <cstdio>
#include <iostream>

namespace hikari {
    /*
        Static methods and fields
    */
    LogLevel Log::reportingLevel = info;
    std::ostream & Log::outputStream = std::cout;

    LogLevel & Log::getReportingLevel() {
        return reportingLevel;
    }

    void Log::setReportingLevel(const LogLevel& level) {
        reportingLevel = level;
    }

    /*
        Non-static methods and fields
    */
    Log::Log()
        : os()
    {

    }

    Log::~Log() {
        Log::outputStream << os.str() << std::endl;
        //fprintf(stdout, "%s", os.str().c_str());
        //fflush(stdout);
    }

    const std::string Log::getNowTime() const {
        std::stringstream ss;
        std::time_t t = std::time(NULL);

        char mbstr[100];
        std::fill_n(mbstr, 100, 0);

#if (_WIN32 && _MSC_VER)
        #pragma warning(push)
        #pragma warning(disable:4996)
        // We disable this warning for portability's sake.
#endif

        if(std::strftime(mbstr, 100, "%H:%M:%S", std::localtime(&t))) {
            ss << mbstr;
        }
#if (_WIN32 && _MSC_VER)
        #pragma warning(pop)
#endif
        return ss.str();
    }

    const std::string Log::toString(const LogLevel & level) const {
        switch(level) {
        case fatal:
            return "FATAL";
        case error:
            return "ERROR";
        case warning:
            return "WARNING";
        case script:
            return "SCRIPT";
        case info:
            return "INFO";
        case debug:
            return "D0";
        case debug1:
            return "D1";
        case debug2:
            return "D2";
        case debug3:
            return "D3";
        case debug4:
            return "D4";
        default:
            return "UNKNOWN";
        }
    }

    std::ostringstream& Log::get(LogLevel level) {
        os << "- " << getNowTime();
        os << " [" << toString(level) << "]: ";

        return os;
    }

} // hikari
