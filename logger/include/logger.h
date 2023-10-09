#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <cstdint>
#include <fstream>

#include <logger_utils.h>

namespace logger{

    // These macros are log functions for users. It eases to use of the class for different level log operations
    #define LogFatal        *(Logger::getInstance()->set_log_level(enumLogLevel::FATAL,     __FILE__, __LINE__))
    #define LogError        *(Logger::getInstance()->set_log_level(enumLogLevel::ERROR,     __FILE__, __LINE__))
    #define LogAlert        *(Logger::getInstance()->set_log_level(enumLogLevel::ALERT,     __FILE__, __LINE__))
    #define LogWarning      *(Logger::getInstance()->set_log_level(enumLogLevel::WARNING,   __FILE__, __LINE__))
    #define LogInfo         *(Logger::getInstance()->set_log_level(enumLogLevel::INFO,      __FILE__, __LINE__))
    #define LogDebug        *(Logger::getInstance()->set_log_level(enumLogLevel::DEBUG,     __FILE__, __LINE__))
    #define LogTrace        *(Logger::getInstance()->set_log_level(enumLogLevel::TRACE,     __FILE__, __LINE__))
    #define HomeDir         get_home_dir()
    
    /**
     * @class Logger
     * 
     * @brief This class can log the messages into console or files with predefined format.
     * 
     * This class uses Singleton pattern so one instance can be used throught out the application for log operations.
     * Thread safety mechanism exists, and to enable it by calling set_thread_safety(true).
     * 
    */
    class Logger{
    private:
        static Logger *ptr_instance_;
        static std::mutex mutex_;
        static structLogFormat *fmt_;

        static std::string config_file_path_;
        static std::string out_file_dir_;
        static std::string out_filename_;

        static enumLogLevel current_level_;

        static bool is_output_set_;
        static bool is_configure_set_;
        static bool is_thread_safe_;
        static bool color_enabled_;
        static bool is_source_enabled_;

        static uint64_t err_counter_;

        static std::stringstream sstream_;
        static std::ofstream log_file_;

        static std::string log_source_file_;
        static int log_source_line_;

        static std::map<enumLogLevel, structLogLevel> mapLogLevel;

        /**
         * @brief This private function creates log string that is ready to either write console or file.
         * 
         * According to content of msg_log content, it adds message fields (Timetamp, Log Level, Log Content etc.) to the output string.
         * 
         * @param[in] msg_log Log message structure to be logged
         * 
         * @return Output log message string
        */
        static std::string log_out_(const structLogMsg &msg_log);
        
        /**
         * @brief This private function writes log message structure to system's console.
         * 
         * @param[in] msg_log Message structure to be logged
         * 
        */
        static void write_console_(const structLogMsg &msg_log);
        
        /**
         * @brief This private function writes log message structure to defined log file.
         * 
         * @param[in] msg_log Message structure to be logged
         * 
        */
        static void write_file_(const structLogMsg &msg_log);

        /**
         * @brief This private function handles internal log errors and prints to the console.
         * 
         * @param[in] err_msg Error message
         * 
        */
        static void error_handler_(const std::string &err_msg);
        
        /**
         * @brief This private function picks color according to log level, and returns corresponding escape characters.
         * 
         * @param[in] log_level Enumeration of log level
         * 
         * @return Escape characters of color code 
         * 
        */
        static inline std::string pick_log_color_(enumLogLevel log_level);
        
        /**
         * @brief This private function sets output log file.
         * 
         * @param[in] filename Filename of output log file. No need to define file extension. ".log" is default format.
         * @param[in] file_dir Directory of output log file. Leave empty if home location is desired. 
         * 
        */
        static void set_output_(const std::string &filename, const std::string file_dir="");

    protected:
    
        /**
         * @brief Constructor of the function. 
        */
        Logger();
        
        /**
         * @brief Destructor of the function. 
        */
        ~Logger();

    public:
            
        /**
         * @brief Copy constructor cannot be exist in Singleton pattern. 
        */
        Logger(const Logger &obj) = delete;

        /**
         * @brief Assignment operator cannot be exist in Singleton pattern. 
        */
        void operator=(const Logger &obj) = delete;

        /**
         * @brief This function returns Singleton Logger object. 
         * 
         * @return Pointer of Logger object
        */
        static Logger *getInstance();
        
        /**
         * @brief This function logs input message. 
         * 
         * @param[in] s_ Input message
         * 
         * @return Pointer of Logger object
         * 
        */
        Logger *operator<<(char *s_);
                
        /**
         * @brief This function logs input message. 
         * 
         * @param[in] s_ Input message
         * 
         * @return Pointer of Logger object
         * 
        */
        Logger *operator<<(const std::string &s_);

        /**
         * @brief This function logs input message. 
         * 
         * @param[in] s_ Input message
         * 
         * @return Pointer of Logger object
         * 
        */
        Logger *operator<<(std::string &s_);
        
        /**
         * @brief This function logs input message. 
         * 
         * @param[in] s_ Input message
         * 
         * @return Pointer of Logger object
        */
        Logger *operator<<(const char *s_);
        
        /**
         * @brief This function configure Logger class with provided configuration file. 
         * 
         * @param[in] file_path Path of configuration file 
         * 
        */
        static void configure(const std::string file_path);
        
        /**
         * @brief This function configure Logger class with provided configuration file. 
         * 
         * @param[in] file_path Path of configuration file 
         * 
        */
        static void configure(char *file_path);
        
        /**
         * @brief This function sets log level of messages. 
         * 
         * Additional arguments (file and line) is used for traceback purposes of log message.
         * 
         * @param[in] level Log level enumeration of message (enumLogLevel::ERROR, enumLogLevel::INFO, etc.). 
         * @param[in] file File path where the function is called. 
         * @param[in] line Line number where the function is called. 
         * 
         * @return Pointer of Logger object
         * 
        */
        static Logger *set_log_level(enumLogLevel level, const char *file=nullptr, int line=-1) noexcept;

        /**
         * @brief This function sets output log file.
         * 
         * @param[in] filename Filename of output log file. No need to define file extension. ".log" is default format.
         * @param[in] file_dir Directory of output log file. Leave empty if home location is desired. 
         * 
        */
        static void set_output(const std::string &filename, const std::string file_dir="");
        
        /**
         * @brief This function sets output log file.
         * 
         * @param[in] filename Filename of output log file. No need to define file extension. ".log" is default format.
         * @param[in] file_dir Directory of output log file. Leave empty if home location is desired. 
         * 
        */
        static void set_output(const char *filename, const char *file_dir=nullptr);
        
        /**
         * @brief This function enables color feature of logs in console.
         * 
        */
        static void enable_colors() noexcept;
        
        /**
         * @brief This function disables color feature of logs in console.
         * 
        */
        static void disable_colors() noexcept;
        
        /**
         * @brief This function sets thread safety by enabling or disabling the mutex.
         * 
        */
        static void set_thread_safety(bool is_safe) noexcept;
        
        /**
         * @brief This function enables source field in the log.
         * 
         * Source Field defines the location of where the log function is called.
         * 
        */
        static void enable_source() noexcept;
        
        /**
         * @brief This function disables source field in the log.
         * 
         * Source Field defines the location of where the log function is called.
         * 
        */
        static void disable_source() noexcept;
        
        /**
         * @brief This function returns setted path of output log file.
         * 
         * If log path has not been set yet, returns empty string. 
         * 
        */
        static std::string get_log_path() noexcept;
    };
}

#endif  // LOGGER_H