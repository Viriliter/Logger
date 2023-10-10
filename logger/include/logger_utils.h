#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#include <string>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdarg>
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#include <ShlObj.h>
#elif __linux__ || __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#include "logger_defs.h"
#include "logger_format.h"

namespace logger{
    
    /**
     * @brief This function returns timestamp information with provided format
     * 
     * @param[in] ts Timestamp structure
     * @param[in] format Format of output timestamp (e.g %Y-%m-%d %H:%M:%S.%f)
     *  
     * @return Output timestamp information 
     *  
    */
    static std::string format_time(const structTimestamp &ts, const std::string &format) {   
        // Use a stringstream to build the formatted string
        std::ostringstream formattedString;
        size_t pos = 0;

        while (pos < format.length()) {
            // Find the next placeholder
            size_t placeholderPos = format.find("%", pos);

            // If no more placeholders are found, append the remaining text
            if (placeholderPos == std::string::npos) {
                formattedString << format.substr(pos);
                break;
            }

            // Append text before the placeholder
            formattedString << format.substr(pos, placeholderPos - pos);

            // Handle the placeholder
            char placeholder = format[placeholderPos + 1];
            switch (placeholder) {
                case 'Y':
                    formattedString << std::setfill('0') << std::setw(4) << 1900 + ts.tm_year;
                    break;
                case 'm':
                    formattedString << std::setfill('0') << std::setw(2) << ts.tm_mon;
                    break;
                case 'd':
                    formattedString << std::setfill('0') << std::setw(2) << ts.tm_mday;
                    break;
                case 'H':
                    formattedString << std::setfill('0') << std::setw(2) << ts.tm_hour;
                    break;
                case 'M':
                    formattedString << std::setfill('0') << std::setw(2) << ts.tm_min;
                    break;
                case 'S':
                    formattedString << std::setfill('0') << std::setw(2) << ts.tm_sec;
                    break;
                case 'f':
                    formattedString << std::setfill('0') << std::setw(3) << ts.tm_msec;
                    break;
                default:
                    // Handle unsupported placeholders or just include the placeholder character
                    formattedString << "%" << placeholder;
            }

            // Move the position past the placeholder
            pos = placeholderPos + 2;
        }

        return formattedString.str();
    }
    
    /**
     * @brief This function returns current timetamp as struct.
     * 
     * @param[in] ts Current timestamp struct
     *  
    */        
    inline void get_current_timestamp_struct(structTimestamp &ts){
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        auto tm_msec = (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000).count();
        struct tm timeInfo;
        #ifdef PLATFORM_WINDOWS
            localtime_s(&timeInfo, &time);
        #else
            localtime_r(&time, &timeInfo);
        #endif
        
        ts = {tm_msec, 
              timeInfo.tm_sec,
              timeInfo.tm_min,
              timeInfo.tm_hour,
              timeInfo.tm_mday,
              timeInfo.tm_mon,
              timeInfo.tm_year,
              timeInfo.tm_wday,
              timeInfo.tm_yday,
              timeInfo.tm_isdst};
    }

    /**
     * @brief This function returns current timetamp information with provided log format.
     * 
     * @param[in] timestamp Current timestamp information
     * @param[in] fmt Timestamp format (e.g "%Y-%m-%d %H:%M:%S.%f")
     *  
    */
    inline void get_current_timestamp(std::string &timestamp, const std::string &fmt){
        std::string ts_format;
        if (!fmt.empty())  ts_format = fmt;
        else  ts_format = DEFAULT_TIMESTAMP_FORMAT;
        
        structTimestamp ts;
        get_current_timestamp_struct(ts);
        
        timestamp = format_time(ts, ts_format);
    }

    /**
     * @brief This function returns format of the provided file path.
     * 
     * @param[in] file_path File path
     * @param[in] format File format with "." prefix
     *
     * @return Position of where file extension is started. If no extension is found returns -1.
     *   
    */
    inline int get_file_format(const std::string &file_path, std::string &format){
        size_t searchPos = file_path.rfind(".");
        if (searchPos == std::string::npos){
            return -1;
        }
        format = file_path;
        
        format = format.substr(searchPos);
        return (int) searchPos;
    }

    /**
     * @brief This function strips the left most delimiter from the message which is provided by log format.
     * 
     * @param[in] msg Message
     * @param[in] fmt Log format structure. If log format is not defined, uses default delimiter type.
     *
    */
    static void lstrip_delimiter(std::string &msg, structLogFormat &fmt){
        char delimiter;
        if (fmt.fmt_delimiter_type!=enumDelimiterType::DEFAULT) delimiter = (char) fmt.fmt_delimiter_type;
        else delimiter = (char) DEFAULT_DELIMITER_TYPE;

        msg.erase(msg.begin(), std::find_if(msg.begin(), msg.end(), [delimiter](char c){
            return (bool) (c!=delimiter);
        }));
    }

    /**
     * @brief This function adds padding to the message according to provided log format.
     * 
     * @param[in] msg Message
     * @param[in] fmt Log format structure If log format is not defined, uses default padding size.
     *
    */
    static void add_padding(std::string &msg, structLogFormat &fmt){
        size_t padding_size;
        if (fmt.fmt_padding_size!=enumPaddingSize::DEFAULT) padding_size = (std::size_t) fmt.fmt_padding_size;
        else padding_size = (std::size_t) DEFAULT_PADDING_SIZE;
        for(int i=0;i<padding_size;i++){
            msg += " ";
        }
    }
            
    /**
     * @brief This function adds delimiter to the message according to provided log format.
     * 
     * @param[in] msg Message
     * @param[in] fmt Log format structure If log format is not defined, uses default padding size.
     *
    */
    static void add_delimiter(std::string &msg, structLogFormat &fmt){
        char delimiter;
        if (fmt.fmt_delimiter_type!=enumDelimiterType::DEFAULT) delimiter = (char) fmt.fmt_delimiter_type;
        else delimiter = (char) DEFAULT_DELIMITER_TYPE;

        msg.push_back(delimiter);
    }
    
    /**
     * @brief This function adds new line to the message.
     * 
     * Since new line mechanisms differs in Windows, MacOS, and Linux. The function adds appropriate CR-LF characters.
     * 
     * @param[in] msg Message
     *
    */
    static void add_newline(std::string &msg){
        std::string newline;
        #if defined(PLATFROM_WINDOWS)
            newline = "\r\n";
        #elif defined(PLATFORM_MACOS)
            newline = "\n";
        #elif defined(PLATFORM_LINUX)
            newline = "\n";      
        #else  // PLATFORM_UNKNOWN
            newline = "\n";
        #endif
        
        msg += newline;
    }

    /**
     * @brief This function adds new field to the output string.
     * 
     * Field is a part of log message seperated from other fields by delimiters
     * 
     * @param[in] field New field to add
     * @param[in] out Output string
     * @param[in] fmt Log format structure
     *
    */
    static void add_field(const std::string &field, std::string &out, structLogFormat &fmt){
        if (field.size()<=0) return;

        add_padding(out, fmt);
        add_delimiter(out, fmt);
        out += field;
    }

    /**
     * @brief This function reads configuration file and updates log format structure.
     * 
     * @param[in] file_path File path of the configuration file 
     * @param[in] fmt Log format structure
     *
    */
    inline void read_config_file(std::string &file_path, structLogFormat *fmt){
        std::string format;
        get_file_format(file_path, format);
        if (format!=".yaml") return;
    }

    /**
     * @brief This function get home location of operating system.
     * 
     * @return Home path. If no home path is found, returns empty string.
     *
    */
    inline static std::string get_home_dir(){
        #if defined(PLATFORM_WINDOWS)
            char userProfile[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_PROFILE, nullptr, 0, userProfile))) {
                return std::string(userProfile);
            }
            else{
                return "";
            }
        #elif defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
            const char* homePath = getenv("HOME");
            if(homePath!=nullptr){
                return std::string(homePath);
            }
            else {
                struct passwd* pwd = getpwuid(getuid());
                if(pwd!=nullptr && pwd->pw_dir != nullptr)
                    return std::string(pwd->pw_dir);
                else
                    return "";
            }
        #else
            return "";
        #endif
    }
}

#endif // LOGGER_UTILS_H