cmake_minimum_required(VERSION 3.17..3.22)
project(PYQUIRC VERSION 0.1.0 LANGUAGES C CXX)



add_library(
    libquirc
    quirc/lib/quirc.c
    quirc/lib/decode.c
    quirc/lib/identify.c
    quirc/lib/version_db.c
)
target_include_directories(libquirc PUBLIC quirc/lib)
target_compile_definitions(libquirc PRIVATE QUIRC_MAX_REGIONS=65534)
set_property(TARGET libquirc PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET libquirc PROPERTY OUTPUT_NAME "quirc")


add_library(
    quirc++
    src/qr.cpp
    src/decode.cpp
)
target_link_libraries(quirc++ PUBLIC libquirc)
target_include_directories(quirc++ PUBLIC include)
target_compile_features(quirc++ PRIVATE cxx_std_14)
set_property(TARGET quirc++ PROPERTY POSITION_INDEPENDENT_CODE ON)

add_executable(example src/main.cpp)
target_link_libraries(example PRIVATE quirc++)
target_compile_features(example PRIVATE cxx_std_14)

find_package(pybind11 REQUIRED)
pybind11_add_module(quirc src/pyquirc.cpp)
target_link_libraries(quirc PRIVATE quirc++)
