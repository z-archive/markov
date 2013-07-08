cmake_minimum_required(VERSION 2.8)
project(Markov CXX)

ADD_DEFINITIONS(
    -std=c++11
)

#message(STATUS "Build type (use -DCMAKE_BUILD_TYPE=[RelWithDebugInfo]/Debug/Release): ${CMAKE_BUILD_TYPE}")
#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE RelWithDebugInfo)
#endif(NOT CMAKE_BUILD_TYPE)


# Dependencies
find_package(Boost REQUIRED thread program_options system serialization)
if(NOT Boost_FOUND)
 message(FATAL_ERROR "Could not find any version of boost")
endif(NOT Boost_FOUND)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

# Includes
include_directories(${CMAKE_SOURCE_DIR})

# Source code
add_subdirectory(common)
add_subdirectory(learn)
add_subdirectory(generate)
add_subdirectory(unittest)

#set(markov_lib_src "*.cpp")
#file(GLOB markov_include "*.h")
#add_library(markov_lib STATIC ${markov_lib_src})
