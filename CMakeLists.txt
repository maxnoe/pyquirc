cmake_minimum_required(VERSION 3.17..3.22)

project(Quirc++ VERSION 0.1.0 LANGUAGES C CXX)

include(GNUInstallDirs)

option(CPP_EXAMPLE "Build the C++ example program" ON)

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
    quirc
    extern/quirc/lib/quirc.c
    extern/quirc/lib/decode.c
    extern/quirc/lib/identify.c
    extern/quirc/lib/version_db.c
)
target_include_directories(quirc PUBLIC )
target_include_directories(
    quirc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/quirc/lib>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


target_compile_definitions(quirc PRIVATE QUIRC_MAX_REGIONS=65534)
set_property(TARGET quirc PROPERTY POSITION_INDEPENDENT_CODE ON)


add_library(
    quirc++
    src/qr.cpp
    src/decode.cpp
)
target_link_libraries(quirc++ PUBLIC quirc)
target_include_directories(
    quirc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(quirc++ PUBLIC cxx_std_11)
set_target_properties(quirc++ PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)


if(CPP_EXAMPLE)
    add_executable(quirc_cpp_example src/main.cpp)
    target_link_libraries(quirc_cpp_example PRIVATE quirc++)
    target_compile_features(quirc_cpp_example PRIVATE cxx_std_11)
    set_target_properties(quirc_cpp_example PROPERTIES
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
    )
endif()

find_package(Python3 REQUIRED COMPONENTS Development Interpreter)
find_package(pybind11 REQUIRED)

pybind11_add_module(pyquirc src/pyquirc.cpp)
target_link_libraries(pyquirc PRIVATE quirc++)
target_compile_features(pyquirc PRIVATE cxx_std_11)
set_target_properties(pyquirc PROPERTIES
    OUTPUT_NAME quirc
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

if(SKBUILD)
    install(TARGETS pyquirc DESTINATION .)
else()
    install(
        TARGETS quirc quirc++ pyquirc
        EXPORT Quirc++Config
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    install(DIRECTORY include/quirc++ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES extern/quirc/lib/quirc.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    include(CMakePackageConfigHelpers)
    export(
        TARGETS quirc quirc++
        NAMESPACE quirc
        FILE "${CMAKE_CURRENT_BINARY_DIR}/FibonacciConfig.cmake"
    )
    write_basic_package_version_file(
        Quirc++ConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
    )
    install(
        EXPORT Quirc++Config
        DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/quirc++"
    )
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/Quirc++ConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/quirc++"
    )

endif()
