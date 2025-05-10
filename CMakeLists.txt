project(mymuduo)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(. SRC_LIST)

add_library(mymuduo SHARED ${SRC_LIST})