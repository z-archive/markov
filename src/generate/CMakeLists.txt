file(GLOB generate_include "*.h")
file(GLOB generate_src "*.cpp")
add_executable(generate ${generate_include} ${generate_src})
target_link_libraries(generate common ${Boost_LIBRARIES})
