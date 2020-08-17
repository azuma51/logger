#pragma once

#include <cstdarg>
#include <string>
#include <iostream>

//LEVEL, toLevelString, Form, Replace
namespace Log {

typedef int LEVEL;
const LEVEL LEVEL_DEBUG    = 10;
const LEVEL LEVEL_INFO     = 20;
const LEVEL LEVEL_WARNING  = 30;
const LEVEL LEVEL_ERROR    = 40;
const LEVEL LEVEL_CRITICAL = 50;
const LEVEL LEVEL_NOUSE    = 100;

static const char* toLevelString(LEVEL lv)
{
    switch (lv) {
    case LEVEL_DEBUG   : return "DEBUG";
    case LEVEL_INFO    : return "INFO";
    case LEVEL_WARNING : return "WARNING";
    case LEVEL_ERROR   : return "ERROR";
    case LEVEL_CRITICAL: return "CRITICAL";
    default      : return "?????";
    };
}





static std::string Form(const char* fmt, ...)
{
    //return CString::Form(fmt, args ...)
    va_list args;
    std::string buff;
    assert(fmt);
    va_start(args, fmt);
    try {
        int len = vsnprintf(NULL, 0, fmt, args);
        if (len < 0)
            throw std::domain_error(fmt);
        buff.assign(len+1, '\0');
        int r = vsprintf((char*)&buff[(size_t)0], fmt, args);
        if (r < 0)
            throw std::domain_error(fmt);
        buff.erase(len, 1);
    }
    catch (...) {
        va_end(args);
        throw;
    }
    va_end(args);
    return buff;
}

static std::string Replace(
        std::string src,
        const std::string& target,
        const std::string& replace)
{
    //return CString::Replace(src, target, replace)
    std::string::size_type pos=0;
    pos = src.find(target);
    while (pos != std::string::npos) {
        src.replace(pos, target.length(), replace);
        pos += replace.length();
        pos = src.find(target, pos);
    }
    return src;
}



};

