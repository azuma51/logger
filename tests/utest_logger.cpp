#include <gtest/gtest.h>

#include "LoggingLogger.h"
using namespace Log;


#define COUNTOF(x)  (sizeof(x)/sizeof(x[0]))

//may be replace {ReplaceTags}, Tests are rough
TEST(Logger, tagName)
{
    std::stringstream o;
    Logger logger("test", LEVEL_DEBUG, o);
    logger.setFormat("{name}");
    logger.debug("debug");

    EXPECT_STREQ("test", o.str().c_str());
}

TEST(Logger, tagLevel)
{
    std::stringstream o;
    Logger logger("test", LEVEL_DEBUG, o);
    logger.setFormat("{level}");
    logger.debug("debug");

    EXPECT_STREQ("DEBUG", o.str().c_str());
}



TEST(Logger, log)
{
    std::stringstream o;
    Logger logger("test");
    logger.setOutput(o);
    logger.setFormat("{message}/");

    typedef struct {
        LEVEL lv;
        std::string r;
    } tagResult;

    tagResult a[] = {
        { LEVEL_NOUSE,    "" },
        { LEVEL_DEBUG,    "debug/info/warning/error/critical/" },
        { LEVEL_INFO,     "info/warning/error/critical/" },
        { LEVEL_WARNING,  "warning/error/critical/" },
        { LEVEL_ERROR,    "error/critical/" },
        { LEVEL_CRITICAL, "critical/" }
    };

    for (unsigned int i=0; i<COUNTOF(a); ++i) {
        logger.setLevel(a[i].lv);
        logger.debug("debug");
        logger.info("info");
        logger.warning("warning");
        logger.error("error");
        logger.critical("critical");

        EXPECT_STREQ(a[i].r.c_str(), o.str().c_str());
        o.str(""); //clear
    }
}

