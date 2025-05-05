project(mymuduo)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(SRC_LIST
    Channel.cc 
    InteAddress.cc 
    logger.cc 
    Timestamp.cc 
)

add_library(mymuduo SHARED ${SRC_LIST})