#ifndef LOGGER_DEFS_H
#define LOGGER_DEFS_H

#include <map>
#include <string>


// Define platform-specific macros
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #define PLATFORM_MACOS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#else
    #define PLATFORM_UNKNOWN
#endif

namespace logger{

    // This section defines the escape characters of color codes for console output
    #define COLOR_RESET      "\x1B[0m"
    #define COLOR_BLACK      "\x1B[30m"
    #define COLOR_RED        "\x1B[31m"
    #define COLOR_GREEN      "\x1B[32m"
    #define COLOR_YELLOW     "\x1B[33m"
    #define COLOR_BLUE       "\x1B[34m"
    #define COLOR_MAGENTA    "\x1B[35m"
    #define COLOR_CYAN       "\x1B[36m"
    #define COLOR_WHITE      "\x1B[37m"

    #define COLOR_BRIGHT_BLACK   "\x1B[90m"  // or "\x1B[1;30m"
    #define COLOR_BRIGHT_RED     "\x1B[91m"  // or "\x1B[1;31m"
    #define COLOR_BRIGHT_GREEN   "\x1B[92m"  // or "\x1B[1;32m"
    #define COLOR_BRIGHT_YELLOW  "\x1B[93m"  // or "\x1B[1;33m"
    #define COLOR_BRIGHT_BLUE    "\x1B[94m"  // or "\x1B[1;34m"
    #define COLOR_BRIGHT_MAGENTA "\x1B[95m"  // or "\x1B[1;35m"
    #define COLOR_BRIGHT_CYAN    "\x1B[96m"  // or "\x1B[1;36m"
    #define COLOR_BRIGHT_WHITE   "\x1B[97m"  // or "\x1B[1;37m"

    #define COLOR_BG_BLACK   "\x1B[40m"
    #define COLOR_BG_RED     "\x1B[41m"
    #define COLOR_BG_GREEN   "\x1B[42m"
    #define COLOR_BG_YELLOW  "\x1B[43m"
    #define COLOR_BG_BLUE    "\x1B[44m"
    #define COLOR_BG_MAGENTA "\x1B[45m"
    #define COLOR_BG_CYAN    "\x1B[46m"
    #define COLOR_BG_WHITE   "\x1B[47m"

    #define COLOR_BG_BRIGHT_BLACK    "\x1B[100m"
    #define COLOR_BG_BRIGHT_RED      "\x1B[101m"
    #define COLOR_BG_BRIGHT_GREEN    "\x1B[102m"
    #define COLOR_BG_BRIGHT_YELLOW   "\x1B[103m"
    #define COLOR_BG_BRIGHT_BLUE     "\x1B[104m"
    #define COLOR_BG_BRIGHT_MAGENTA  "\x1B[105m"
    #define COLOR_BG_BRIGHT_CYAN     "\x1B[106m"
    #define COLOR_BG_BRIGHT_WHITE    "\x1B[107m"

    // LOG OPTIONS
    #define SHOW_SOURCE_INFO 1
    #define WRITE_FILE 2
    #define MASK_SHOW_SOURCE_INFO 0x01
    #define MASK_WRITE_FILE 0x02
    
    typedef struct {std::string desc; std::string color; uint16_t option=0x0000;} structLogLevel;

    /**
     * @enum enumLogLevel
     * 
     * @brief This enum defines log levels
    */
    enum class enumLogLevel{
        FATAL_ = 0,
        ERROR_ = 1,
        ALERT_ = 2,
        WARNING_ = 3,
        INFO_ = 4,
        DEBUG_ = 5,
        TRACE_ = 6,
        LOG_ERROR_ = 7,
        INVALID_ = 8
    };

    /**
     * @enum enumDelimiterType
     * 
     * @brief This enum defines delimiter types
    */
    enum class enumDelimiterType: char{
        DEFAULT,
        TAB = '\t',
        SPACE = ' ',
        COMMA = ',',
        DASH = '-',
    };

    /**
     * @enum enumPaddingSize
     * 
     * @brief This enum defines padding sizes
    */        
    enum class enumPaddingSize{
        DEFAULT = -1,
        ZERO = 0,
        ONE = 1,
        TWO = 2,
        THREE = 3,
        FOUR = 4,
        EIGHT = 8,
        CUSTOM = -1
    };

    /**
     * @struct structLogFormat
     * 
     * @brief This struct defines format of log messages
    */
    struct structLogFormat{
        std::string fmt_timestamp = "";                                     ///< Timestamp format
        enumDelimiterType fmt_delimiter_type = enumDelimiterType::DEFAULT;  ///< Delimiter type
        enumPaddingSize fmt_padding_size = enumPaddingSize::DEFAULT;        ///< Padding size
    };

    /**
     * @struct structTimestamp
     * 
     * @brief This struct defines the timestamp
     * 
     * It has similar structure with native time_t structure 
     * 
    */
    struct structTimestamp
    {
        long int tm_msec=0;	///< Miliseconds.	[0-1000]
        int tm_sec=0;		///< Seconds.	[0-60] (1 leap second)
        int tm_min=0;		///< Minutes.	[0-59]
        int tm_hour=0;		///< Hours.	[0-23]
        int tm_mday=0;		///< Day.		[1-31]
        int tm_mon=0;		///< Month.	[0-11]
        int tm_year=0;		///< Year	- 1900.
        int tm_wday=0;		///< Day of week.	[0-6]
        int tm_yday=0;		///< Days in year.[0-365]
        int tm_isdst=0;		///< DST.		[-1/0/1]
    };

    /**
     * @struct structLogMsg
     * 
     * @brief This struct defines the content of log messages
    */   
    struct structLogMsg{
        structTimestamp timestamp;      ///< Timestamp
        enumLogLevel log_level;     ///< Log Level
        std::string log_level_desc; ///< Custom Log Level Description
        std::string source;         ///< Source of Log Message
        std::string msg;            ///< Message
    };
}

#endif  // LOGGER_DEFS_H