cmake_minimum_required(VERSION 3.19)
project(RS_Image_Processing)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(RS_Image_Processing
        src/image.cpp
        include/image.h
        src/main.cpp include/correct.h src/correct.cpp)

# Eigen
find_package(Eigen3 REQUIRED)

#QT
find_package(Qt5 COMPONENTS Widgets Gui LinguistTools REQUIRED)

# GDAL
find_package(GDAL REQUIRED)
if (NOT GDAL_FOUND)
    message(FATAL_ERROR "GDAL not found, cannot proceed")
endif ()
if (NOT GDAL_CONFIG)
    message(FATAL_ERROR "gdal-config command not found (not in PATH?), cannot proceed")
endif ()

execute_process(
        COMMAND ${GDAL_CONFIG} --version
        OUTPUT_VARIABLE SYSTEM_GDAL_VERSION
)

if (SYSTEM_GDAL_VERSION VERSION_LESS "2.2")
    message(FATAL_ERROR "GDAL version \"${SYSTEM_GDAL_VERSION}\" is too old, at least 2.2 is required")
endif ()

target_link_libraries(
        RS_Image_Processing
        Qt${QT_VERSION_MAJOR}::Widgets
        ${GDAL_LIBRARY}
        Eigen3::Eigen
        )