#include "hikari/core/util/Log.hpp"
#include <ctime>
#include <cstdio>
#include <iostream>

namespace hikari {
    /*
        Static methods and fields
    */
    TLogLevel Log::reportingLevel = info;

    TLogLevel & Log::getReportingLevel() {
        return reportingLevel;
    }

    void Log::setReportingLevel(const TLogLevel& level) {
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
        os << std::endl;
        fprintf(stdout, "%s", os.str().c_str());
        fflush(stdout);
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

    const std::string Log::toString(const TLogLevel & level) const {
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
            return "DEBUG";
        case debug1:
            return "DEBUG1";
        case debug2:
            return "DEBUG2";
        case debug3:
            return "DEBUG3";
        case debug4:
            return "DEBUG4";
        default:
            return "UNKNOWN";
        }
    }

    std::ostringstream& Log::get(TLogLevel level) {
        int tabLevel = level < debug ? 0 : level - debug;

        os << "- " << getNowTime();
        os << " [" << toString(level) << "]:\t";
        os << std::string(tabLevel, '\t');

        return os;
    }

} // hikari
