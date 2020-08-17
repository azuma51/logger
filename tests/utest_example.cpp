#include <gtest/gtest.h>

#include "Logging.h"
using namespace Log;


class Fuzz {
public:
    Logger* logger;

    void a()
    {
        logger->info("info");
        logger->debug("debug");
        logger->warning("warning");
        logger->error("what you get if you multiply six by nine = %d", 42);
        logger->critical("%s", "oops!");
    }

    Fuzz()
    {
        logger = Logging::getLogger("Fuzz");
        logger->setLevel(Logging::WARNING);
        logger->setOutput(std::cout);
        logger->setFormat("[{datetime}] {lv}:{name}> {message}\n");
    }

    ~Fuzz()
    {
        Logging::removeLogger(logger);

        Logging::setLevel(Logging::INFO);
        Logging::info("Fuzz::~Fuzz(), remove logger.");
        Logging::ViewAll();
    }
};


TEST(Example, test)
{
    Fuzz f;
    f.a();
}


