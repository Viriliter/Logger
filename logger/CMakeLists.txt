SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_library(libLogger src/logger.cpp )

target_include_directories(libLogger PUBLIC include)