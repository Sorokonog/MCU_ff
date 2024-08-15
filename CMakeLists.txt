cmake_minimum_required(VERSION 3.5.0)
project(ff_flight VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_COMPILER gcc.exe)
set(CMAKE_CXX_COMPILER g++.exe)

include_directories("${PROJECT_SOURCE_DIR}/Inc")
include_directories("${PROJECT_SOURCE_DIR}/tracklib")

add_library(trackerinterface STATIC IMPORTED)
set_property(TARGET trackerinterface PROPERTY
             IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/tracklib/libtrackerinterface.a")

add_executable(ff_flight main.cpp)

target_link_libraries(${PROJECT_NAME} trackerinterface)