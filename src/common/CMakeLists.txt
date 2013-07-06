file(GLOB common_include "*.h")
file(GLOB common_src "*.cpp")
add_library(common ${common_include} ${common_src})

