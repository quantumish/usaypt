# Hewo! This is the CMake file

cmake_minimum_required(VERSION 3.10.0)
project(chainfountain)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(CHRONO_ENGINE NAMES libChronoEngine.so REQUIRED)
find_library(CHRONO_IRRLICHT NAMES libChronoEngine_irrlicht.so REQUIRED)
find_package(Eigen3 REQUIRED)

find_path(CHRONO_INCLUDE NAMES "chrono/ChConfig.h" REQUIRED)
set(CHRONO_INCLUDE "${CHRONO_INCLUDE}/chrono") # Somewhat questionable

find_path(CHRONO_IRRLICHT_INCLUDE NAMES "irrlicht/irrlicht.h" REQUIRED)
set(CHRONO_IRRLICHT_INCLUDE "${CHRONO_IRRLICHT_INCLUDE}/irrlicht") # Somewhat questionable

# TODO EIGEN3_INCLUDE_DIR is not set tho
include_directories(${EIGEN3_INCLUDE_DIR} ${CHRONO_INCLUDE} ${CHRONO_IRRLICHT_INCLUDE})

link_libraries(${CHRONO_ENGINE} ${CHRONO_IRRLICHT})

file( GLOB SOURCES src/*.cpp )
add_executable(ChainFountain main.cpp ${SOURCES})

