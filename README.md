C++ Logger
----------

Python inspired c++11 logger.


### Output quickly

Logging is singleton.

```c++
#include <Logging.h>
using namespace Log;

void a() {
  Logging::setLevel(Logging::INFO);
  Logging::info("info");
  Logging::debug("debug");
}
```

### Managed logger

Logging is logger manager.

```c++
#include <Logging.h>
using namespace Log;

class Fuzz {
public:
    Logger* logger;
    
    void a()
    {
        logger->warning("warning");
        logger->error("what you get if you multiply six by nine = %d", 42);
        logger->critical("%s", "oops!");
    }

    Fuzz()
    {
        logger = Logging::getLogger("Fuzz");
        logger->setLevel(Logging::WARNING);
        logger->setOutput(std::cout);
        logger->setFormat("[{datetime}] {level}:{name}> {message}\n");
    }
    ~Fuzz()
    {
        //if you need can delete
        Logging::removeLogger(logger);
    }
};

```

