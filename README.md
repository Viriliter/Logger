# Logger
Basic logger library.

## Basic Usage
```c++
#include <logger.h>

using namespace logger;

int main(){
    // Enables log color for console depending on its level
    Logger::enable_colors();
    // Creates log file
    Logger::set_output("log_test.txt");

    LogFatal << "This is fatal message.";
    LogError << "This is error message.";
    LogAlert << "This is alert message.";
    LogWarning << "This is warning message.";
    LogInfo << "This is info message.";
    LogDebug << "This is debug message.";
    LogTrace << "This is trace message.";

    return 0;
}
```
For more examples, look ```/example```.

## Install

```bash
$ git clone https://github.com/Viriliter/Logger.git
$ cd /Logger && mkdir build && cd build
$ cmake ../ && make -j
$ sudo make install
```
See example to how to use it.

## Platforms
The library supports C++14 and above.
* Linux
* Windows
* MacOS
