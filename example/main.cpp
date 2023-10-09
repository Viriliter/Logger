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