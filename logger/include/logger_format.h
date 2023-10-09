#ifndef LOGGER_FORMAT_H
#define LOGGER_FORMAT_H

#include <string>
#include "logger_defs.h"

namespace logger{

    // Define default log formats
    #define DEFAULT_TIMESTAMP_FORMAT    "%Y-%m-%d %H:%M:%S.%f";
    #define DEFAULT_DELIMITER_TYPE      enumDelimiterType::TAB;
    #define DEFAULT_PADDING_SIZE        enumPaddingSize::ZERO;
}

#endif // LOGGER_FORMAT_H