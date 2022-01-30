cmake_minimum_required(VERSION 3.17..3.22)
project(PYQUIRC VERSION 0.1.0 LANGUAGES C CXX)

if(SKBUILD)
  # Scikit-Build does not add your site-packages to the search path
  # automatically, so we need to add it _or_ the pybind11 specific directory
  # here.
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c
            "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE _tmp_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
  list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
endif()


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

add_executable(quirc_cpp_example src/main.cpp)
target_link_libraries(quirc_cpp_example PRIVATE quirc++)
target_compile_features(quirc_cpp_example PRIVATE cxx_std_14)

find_package(pybind11 REQUIRED)
pybind11_add_module(quirc src/pyquirc.cpp)
target_link_libraries(quirc PRIVATE quirc++)

install(TARGETS quirc DESTINATION .)
