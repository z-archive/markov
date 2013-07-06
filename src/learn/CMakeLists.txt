file(GLOB learn_include "*.h")
file(GLOB learn_src "*.cpp")
add_executable(learn ${learn_include} ${learn_src})
target_link_libraries(learn common ${Boost_LIBRARIES})
