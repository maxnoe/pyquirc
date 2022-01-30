cmake_minimum_required(VERSION 3.17..3.22)
project(PYQUIRC VERSION 0.1.0 LANGUAGES C CXX)



add_library(
    quirc
    quirc/lib/quirc.c
    quirc/lib/decode.c
    quirc/lib/identify.c
    quirc/lib/version_db.c
)
target_include_directories(quirc PUBLIC quirc/lib)
target_compile_definitions(quirc PRIVATE QUIRC_MAX_REGIONS=65534)


add_library(
    quirc++
    src/qr.cpp
    src/decode.cpp
)
target_link_libraries(quirc++ PUBLIC quirc)
target_include_directories(quirc++ PUBLIC include)
target_compile_features(quirc++ PRIVATE cxx_std_14)

add_executable(example src/main.cpp)
target_link_libraries(example PRIVATE quirc++)
target_compile_features(example PRIVATE cxx_std_14)
