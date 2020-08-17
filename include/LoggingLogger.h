# pragma once

#include "LoggingDef.h"

namespace Log {

#define DEF_LOGGER_LEVEL    LEVEL_WARNING
#define DEF_LOGGER_OUTPUT   std::cout
//#define DEF_LOGGER_FORMAT   "[{datetime}] {level}:{name}>{message}\n"
#define DEF_LOGGER_FORMAT   "[{datetime}] {lv}:{name}> {message}\n"

class Logger
{
public:
    Logger(const char* name,
            LEVEL lv = DEF_LOGGER_LEVEL,
            std::ostream& o = DEF_LOGGER_OUTPUT);
    virtual ~Logger();

    void setLevel(LEVEL);
    void setOutput(std::ostream&);
    void setFormat(const char*);

    const char* getName();
    LEVEL getLevel();
    std::ostream* getOutput();
    const char* getFormat();

    template <typename ... Args>
        void log(LEVEL, const char* fmt, Args const & ... args);

    template <typename ... Args>
        void debug   (const char* fmt, Args const & ... args);
    template <typename ... Args>
        void info    (const char* fmt, Args const & ... args);
    template <typename ... Args>
        void warning (const char* fmt, Args const & ... args);
    template <typename ... Args>
        void error   (const char* fmt, Args const & ... args);
    template <typename ... Args>
        void critical(const char* fmt, Args const & ... args);


protected:
    std::string name;
    int level;
    std::ostream* out;
    std::string format;

    void output(LEVEL, std::string msg);
};


inline Logger::Logger(
        const char* _name,
        LEVEL lv,
        std::ostream& o)
{
    name   = _name;
    level  = lv;
    out    = &o;
    format = DEF_LOGGER_FORMAT;
}

inline Logger::~Logger()
{
}


inline void Logger::setLevel(LEVEL lv)         { level  = lv; }
inline void Logger::setOutput(std::ostream& o) { out    = &o; }
inline void Logger::setFormat(const char* f)   { format = f; }

inline const char*   Logger::getName()   { return name.c_str(); }
inline LEVEL         Logger::getLevel()  { return level; }
inline std::ostream* Logger::getOutput() { return out; }
inline const char*   Logger::getFormat() { return format.c_str(); }


template <typename ... Args>
inline void Logger::log(LEVEL lv, const char* fmt, Args const & ... args)
{
    if (level <= lv)
        output(lv, Form(fmt, args ...));
}

template <typename ... Args>
inline void Logger::debug(const char* fmt, Args const & ... args)
{
    log(LEVEL_DEBUG, fmt, args ...);
}

template <typename ... Args>
inline void Logger::info(const char* fmt, Args const & ... args)
{
    log(LEVEL_INFO, fmt, args ...);
}

template <typename ... Args>
inline void Logger::warning(const char* fmt, Args const & ... args)
{
    log(LEVEL_WARNING, fmt, args ...);
}

template <typename ... Args>
inline void Logger::error(const char* fmt, Args const & ... args)
{
    log(LEVEL_ERROR, fmt, args ...);
}

template <typename ... Args>
inline void Logger::critical(const char* fmt, Args const & ... args)
{
    log(LEVEL_CRITICAL, fmt, args ...);
}


inline void Logger::output(LEVEL lv, std::string msg)
{
    assert(out);

    //ココは必要になったら書式化する
    time_t t = time(NULL);
    struct tm *d = localtime(&t);
    std::string date =
        Form("%04d/%02d/%02d %2d:%02d:%02d",
                d->tm_year+1900,
                d->tm_mon+1,
                d->tm_mday,
                d->tm_hour,
                d->tm_min,
                d->tm_sec);


    std::string b = format;
    b = Replace(b, "{datetime}", date);
    b = Replace(b, "{name}",     name);
    b = Replace(b, "{level}",    toLevelString(lv));
    b = Replace(b, "{lv}",
            std::string(toLevelString(lv)).substr(0, 1));
    b = Replace(b, "{message}",  msg);
    //b = Replace(b, "{msg}", msg);
    *out << b;
}

};//Log
