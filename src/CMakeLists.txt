cmake_minimum_required(VERSION 2.8)
project(Markov CXX)

# Dependencies
find_package(Boost REQUIRED program_options)
if(NOT Boost_FOUND)
 message(FATAL_ERROR "Could not find any version of boost")
endif(NOT Boost_FOUND)

# Source code
add_subdirectory(learn)
add_subdirectory(generate)
add_subdirectory(unittest)

#set(markov_lib_src "*.cpp")
#file(GLOB markov_include "*.h")
#add_library(markov_lib STATIC ${markov_lib_src})
