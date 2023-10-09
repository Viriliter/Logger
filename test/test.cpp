#include <string>
#include <thread>
#include <chrono>

#include <logger.h>

using namespace logger;

int main(){
    
    Logger::enable_colors();
    LogDebug << "This is debug message";
    LogInfo << "This is info message";
    LogWarning << "This is warning message";
    LogError << "This is error message";
    LogFatal << "This is fatal message";
    Logger::enable_source();
    LogDebug << "This is debug message";
    LogTrace << "This is trace message";
    LogWarning << "This is warning message";
    Logger::disable_source();
    LogDebug << "This is debug message";
    LogInfo << "This is info message";
    LogWarning << "This is warning message";
    LogError << "This is error message";
    LogFatal << "This is fatal message";

    Logger::set_output("log_test.txt", HomeDir + "/log-rec");
    LogDebug << "This is debug message";
    LogInfo << "This is info message";
    std::this_thread::sleep_for(std::chrono::milliseconds(60));

    LogWarning << "This is warning message";
    LogError << "This is error message";
    LogFatal << "This is fatal message";
    Logger::set_output("./log_test.txt");
    //LogWarning << HomeDir;
    LogInfo << Logger::get_log_path();
    *(Logger::getInstance()->set_log_level(enumLogLevel::DEBUG_)) << Logger::get_log_path();
    std::string u = "uuuu";
    char g[] = "gnfjd";
    LogError << u;
    LogError << g;
    
    return 0;
}
