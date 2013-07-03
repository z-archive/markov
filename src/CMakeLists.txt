cmake_minimum_required(VERSION 2.8)
project(Markov CXX)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

# Dependencies
find_package(Boost REQUIRED program_options system)
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
