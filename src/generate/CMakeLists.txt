file(GLOB generate_include "*.h")
file(GLOB generate_src "*.cpp")
add_library(generate ${generate_include} ${generate_src})
