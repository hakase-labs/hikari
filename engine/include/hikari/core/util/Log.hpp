#ifndef HIKARI_CORE_UTIL_LOG
#define HIKARI_CORE_UTIL_LOG

#include "hikari/core/Platform.hpp"
#include <iostream>
#include <sstream>

namespace hikari {

/*
    Logging convenience macro
*/
#define HIKARI_LOG(level) \
if(level > ::hikari::Log::getReportingLevel()) {} \
else ::hikari::Log().get(level)

    enum TLogLevel {
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
        static TLogLevel reportingLevel;
        Log(const Log&);
        Log& operator =(const Log&);

        const std::string getNowTime() const;
        const std::string toString(const TLogLevel & level) const;

    protected:
        std::ostringstream os;

    public:
        Log();
        virtual ~Log();
        std::ostringstream& get(TLogLevel level = info);
        static TLogLevel& getReportingLevel();
        static void setReportingLevel(const TLogLevel& level);
    };
    
}

#endif // HIKARI_CORE_UTIL_LOG