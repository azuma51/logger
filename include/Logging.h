#pragma once

#include "LoggingDef.h"
#include "LoggingLogger.h"
#include "LoggingManager.h"

namespace Log {

#define LOGGING_LOGGER  "logging"


//singleton
class Logging
    : protected Manager
{
public:
    typedef Log::LEVEL LEVEL;
    static const LEVEL DEBUG    = LEVEL_DEBUG;
    static const LEVEL INFO     = LEVEL_INFO;
    static const LEVEL WARNING  = LEVEL_WARNING;
    static const LEVEL ERROR    = LEVEL_ERROR;
    static const LEVEL CRITICAL = LEVEL_CRITICAL;
    static const LEVEL NOUSE    = LEVEL_NOUSE; 

public:
    static Logger* getLogger(const char* name);
    static Logger* getLogger(const char* name, LEVEL);
    static Logger* getLogger(const char* name, LEVEL, std::ostream& o);
    static bool removeLogger(Logger* logger);

    static void setFormatAll(const char*);
    static void setOutputAll(std::ostream& o);

    static void ViewAll(std::ostream& o=std::cout);

public:
    //logging.logger
    static void setLevel(LEVEL);
    static void setOutput(std::ostream& o);
    static void setFormat(const char* fmt);

    template <typename ... Args>
        static void log(LEVEL, const char* fmt, Args const & ... args);

    template <typename ... Args>
        static void debug   (const char* fmt, Args const & ... args);
    template <typename ... Args>
        static void info    (const char* fmt, Args const & ... args);
    template <typename ... Args>
        static void warning (const char* fmt, Args const & ... args);
    template <typename ... Args>
        static void error   (const char* fmt, Args const & ... args);
    template <typename ... Args>
        static void critical(const char* fmt, Args const & ... args);

protected:
    static Logging* getInst() {
        static Logging inst; return &inst;
    }

public:
    //call this only tests or debug
    void DeleteAll() {
        Manager::DeleteAll();
    }

//private:
protected:
    Logging();
    ~Logging();
    Logging(const Logging*) {}
    Logging& operator=(const Logging*) { return *this; }
};


inline Logging::Logging() {}
inline Logging::~Logging() {}

inline Logger* Logging::getLogger(const char* name)
{
    return getInst()->get(name);
}

inline Logger* Logging::getLogger(const char* name, LEVEL lv)
{
    Logger* logger = getLogger(name);
    assert(logger);
    logger->setLevel(lv);
    return logger;
}

inline Logger* Logging::getLogger(const char* name, LEVEL lv, std::ostream& o)
{
    Logger* logger = getLogger(name);
    assert(logger);
    logger->setLevel(lv);
    logger->setOutput(o);
    return logger;
}

inline bool Logging::removeLogger(Logger* logger)
{
    return getInst()->remove(logger);
}

inline void Logging::setFormatAll(const char* fmt)
{
    getInst()->Manager::setFormatAll(fmt);
}

inline void Logging::setOutputAll(std::ostream& o)
{
    getInst()->Manager::setOutputAll(o);
}

inline void Logging::ViewAll(std::ostream& o)
{
    getInst()->Manager::ViewAll(o);
}


//logging.logger

inline void Logging::setLevel(LEVEL lv)
{
    getLogger(LOGGING_LOGGER)->setLevel(lv);
}
inline void Logging::setFormat(const char* fmt)
{
    getLogger(LOGGING_LOGGER)->setFormat(fmt);
}
inline void Logging::setOutput(std::ostream& o)
{
    getLogger(LOGGING_LOGGER)->setOutput(o);
}


template <typename ... Args>
inline void Logging::log(LEVEL lv, const char* fmt, Args const & ... args)
{
    getLogger(LOGGING_LOGGER)->log(lv, fmt, args ...);
}

template <typename ... Args>
inline void Logging::debug(const char* fmt, Args const & ... args)
{
    log(DEBUG, fmt, args ...);
}

template <typename ... Args>
inline void Logging::info(const char* fmt, Args const & ... args)
{
    log(INFO, fmt, args ...);
}

template <typename ... Args>
inline void Logging::warning(const char* fmt, Args const & ... args)
{
    log(WARNING, fmt, args ...);
}

template <typename ... Args>
inline void Logging::error(const char* fmt, Args const & ... args)
{
    log(ERROR, fmt, args ...);
}

template <typename ... Args>
inline void Logging::critical(const char* fmt, Args const & ... args)
{
    log(CRITICAL, fmt, args ...);
}


};//Log


