#ifndef HIKARI_CORE_UTIL_LOG
#define HIKARI_CORE_UTIL_LOG

#include "hikari/core/Platform.hpp"
#include <iostream>
#include <sstream>

#ifndef HIKARI_LOG_MAX_LEVEL
#define HIKARI_LOG_MAX_LEVEL 4
#endif

namespace hikari {

/*
    Logging convenience macro
*/
#define HIKARI_LOG(level) \
if(level > HIKARI_LOG_MAX_LEVEL) {} \
else if(level > ::hikari::Log::getReportingLevel()) {} \
else ::hikari::Log().get(level)

    enum LogLevel {
        fatal,
        error,
        warning,
        script,
        info,
        debug,
        debug1,
        debug2,
        debug3,
        debug4
    };

    /*
        Originally adapted from:
        
            "Logging In C++" 
            by Petru Marginean
            September 05, 2007 
            (http://www.drdobbs.com/cpp/logging-in-c/201804215?pgno=1)

    */
    class HIKARI_API Log {
    private:
        static LogLevel reportingLevel;
        static std::ostream & outputStream;
        Log(const Log&);
        Log& operator =(const Log&);

        const std::string getNowTime() const;
        const std::string toString(const LogLevel & level) const;

    protected:
        std::ostringstream os;

    public:
        Log();
        virtual ~Log();
        std::ostringstream& get(LogLevel level = info);
        static LogLevel& getReportingLevel();
        static void setReportingLevel(const LogLevel& level);
    };
    
}

#endif // HIKARI_CORE_UTIL_LOG