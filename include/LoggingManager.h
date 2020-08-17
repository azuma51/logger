#pragma once

#include "LoggingDef.h"
#include "LoggingLogger.h"

#include <cstring>
#include <mutex>

namespace Log {


class Manager
{
public:
    Logger* get(const char* name);
    bool remove(Logger* logger);

    void setFormatAll(const char* fmt);
    void setOutputAll(std::ostream& o);

    void ViewAll(std::ostream& o=std::cout);
    void DeleteAll();

protected:
    std::mutex mtx;
    std::map<std::string, Logger*> loggers;

    Logger* find(const char* key);

protected:
    Manager();
    ~Manager();
};


inline Manager::Manager() {}
inline Manager::~Manager()
{
    DeleteAll();
}

inline Logger* Manager::get(const char* name)
{
    //reject empty naming
    assert(name);
    assert(std::strcmp(name, ""));
    //scoped locking pattern
    std::lock_guard<std::mutex> lock(mtx);

    Logger* logger = find(name);
    if (logger)
        return logger;

    //new
    logger = new Logger(name);
    assert(logger);
    //add
    auto r = loggers.insert(std::make_pair(name, logger));
    if (!r.second) {
        //structurally impossible,
        //but throws an exception when duplicated key
        throw std::logic_error("a duplicate key");
        return nullptr;
    }
    return logger;
}

inline bool Manager::remove(Logger* logger)
{
    assert(logger);
    std::lock_guard<std::mutex> lock(mtx);

    auto it = loggers.find(logger->getName());
    if (it != loggers.end()
        && ((Logger*)it->second == logger))
    {
        //del
        delete logger;
        //remove
        loggers.erase(it);
        return true;
    }
    return false;
}

inline Logger* Manager::find(const char* key)
{
    //std::unique_lock<std::mutex> lock(mtx);
    auto it = loggers.find(key);
    if (it != loggers.end())
        return it->second;
    return nullptr;
}


//setLevelAll not recommended
//since the one set to NOUSE will be revived

inline void Manager::setFormatAll(const char* fmt)
{
    std::lock_guard<std::mutex> lock(mtx);

    for (auto it = loggers.begin();
              it != loggers.end();
              ++it)
        ((Logger*)it->second)->setFormat(fmt);
}

inline void Manager::setOutputAll(std::ostream& o)
{
    std::lock_guard<std::mutex> lock(mtx);

    for (auto it = loggers.begin();
              it != loggers.end();
              ++it)
        ((Logger*)it->second)->setOutput(o);
}


inline void Manager::ViewAll(std::ostream& o)
{
    std::lock_guard<std::mutex> lock(mtx);

    int n = 1;
    for (auto it = loggers.begin();
              it != loggers.end();
              ++it, ++n)
    {
        Logger* logger = it->second;
        o   << std::setw(3) << std::setfill(' ') << n << ": "
            << logger->getName() << "=>"
            << toLevelString(logger->getLevel())
            << std::endl;
    }
}

//call this only tests or debug
inline void Manager::DeleteAll()
{
    std::lock_guard<std::mutex> lock(mtx);

    for(auto it = loggers.begin();
             it != loggers.end();
             ++it)
    {
        delete (Logger*)it->second;
        //loggers.erase(it);
    }
    loggers.clear();
}


};//Log
