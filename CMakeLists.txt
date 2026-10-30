cmake_minimum_required(VERSION 3.18)

# pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

# workaround: pico_sdk_import.cmake seems to incomplete
include_directories($ENV{PICO_SDK_PATH}/src/rp2_common/pico_async_context/include)

# we also need PICO EXTRAS
include(pico_extras_import.cmake)

project(pico-doktorhut)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
add_compile_options(-Wall -Wextra -Wnull-dereference)

# initialize the Pico SDK
pico_sdk_init()

add_executable(pico-doktorhut
  src/doktorhut.cpp)

# pull in common dependencies
target_link_libraries(pico-doktorhut
  pico_stdlib
  hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico-doktorhut)

# Local variables:
#   mode: cmake
#   coding: utf-8
# End:
