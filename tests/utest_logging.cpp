#include <gtest/gtest.h>

#include "Logging.h"
using namespace Log;

#define COUNTOF(x)  (sizeof(x)/sizeof(x[0]))

class LoggingTest :
    public Logging,
    public ::testing::Test
{
protected:
    void SetUp()
    {
        getInst()->DeleteAll();
    }
    void TearDown()
    {
    }
protected:
    LoggingTest() {};
    ~LoggingTest() {};
};


TEST(Logging, example_1)
{
    //output quickly
    Logging::setLevel(Logging::INFO);
    Logging::debug("debug");
    Logging::info("info");
}


TEST(Logging, example_2)
{
    //managed output, file, class, lib, etc...
    Logger* a = Logging::getLogger("path/to/a");
    a->debug("debug");
    a->info("info");

    Logger* b = Logging::getLogger("lib/b", Logging::DEBUG);
    b->debug("debug");
    b->info("info");

    Logger* c = Logging::getLogger("someting/c.h", Logging::NOUSE);
    c->debug("debug");
    c->critical("critical");

    //can delete it, but not recommended
    Logging::removeLogger(c);

    //logger list output
    Logging::ViewAll();
}


TEST_F(LoggingTest, GetRemove)
{
    //same to ManagerTest_GetRemove
}

TEST_F(LoggingTest, setFormatAll)
{
    Logger* a = Logging::getLogger("A");
    Logger* b = Logging::getLogger("B");

    Logging::setFormatAll("{message}");
    EXPECT_STREQ("{message}", a->getFormat());
    EXPECT_STREQ("{message}", b->getFormat());

    Logging::setFormatAll("{level}");
    EXPECT_STREQ("{level}", a->getFormat());
    EXPECT_STREQ("{level}", b->getFormat());
}

TEST_F(LoggingTest, setOutputAll)
{
    Logger* a = Logging::getLogger("A");
    Logger* b = Logging::getLogger("B");

    Logging::setOutputAll(std::cout);
    EXPECT_EQ(&std::cout, a->getOutput());
    EXPECT_EQ(&std::cout, b->getOutput());

    Logging::setOutputAll(std::cerr);
    EXPECT_EQ(&std::cerr, a->getOutput());
    EXPECT_EQ(&std::cerr, b->getOutput());
}


TEST(Logging, log)
{
    std::stringstream o;
    Logging::setOutput(o);
    Logging::setFormat("{message}/");

    typedef struct {
        Logging::LEVEL lv;
        std::string r;
    } tagResult;

    tagResult a[] = {
        { Logging::NOUSE,    "" },
        { Logging::DEBUG,    "debug/info/warning/error/critical/" },
        { Logging::INFO,     "info/warning/error/critical/" },
        { Logging::WARNING,  "warning/error/critical/" },
        { Logging::ERROR,    "error/critical/" },
        { Logging::CRITICAL, "critical/" }
    };

    for (unsigned int i=0; i<COUNTOF(a); ++i) {
        Logging::setLevel(a[i].lv);
        Logging::debug("debug");
        Logging::info("info");
        Logging::warning("warning");
        Logging::error("error");
        Logging::critical("critical");

        EXPECT_STREQ(a[i].r.c_str(), o.str().c_str());
        o.str(""); //clear
    }
}


TEST(Logging, default_argument_fix)
{
    Logger* logger = Logging::getLogger("Fuzz", Logging::WARNING);
    EXPECT_TRUE(Logging::WARNING == logger->getLevel());
    
    logger->setLevel(Logging::ERROR);//set new level

    // re-get logger (FAILED:with default argument level, output)
    logger = Logging::getLogger("Fuzz");
    EXPECT_FALSE(Logging::WARNING == logger->getLevel());
    EXPECT_TRUE(Logging::ERROR == logger->getLevel());
}


TEST(Loggin, threads)
{
    //same to ManagerTest_threads
}
