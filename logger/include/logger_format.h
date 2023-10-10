#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H

#include <string>
#include "logger_defs.h"

namespace logger{

    // Define default log formats
    #define DEFAULT_TIMESTAMP_FORMAT        "%Y-%m-%d %H:%M:%S.%f"
    #define DEFAULT_TIMESTAMP_PREFIX_FORMAT "%Y%m%d"
    #define DEFAULT_DELIMITER_TYPE          enumDelimiterType::TAB
    #define DEFAULT_PADDING_SIZE            enumPaddingSize::ZERO
    #define DEFAULT_LOG_EXTENSION           ".log"
}

#endif // LOGGER_FORMAT_H