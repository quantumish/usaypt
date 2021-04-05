# Hewo! This is the CMake file

cmake_minimum_required(VERSION 3.10.0)
project(chainfountain)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find your very own copy of CHRONO_ENGINE
find_library(CHRONO_ENGINE NAMES libChronoEngine.so REQUIRED)

# find your very own copy of CHRONO_IRRLICHT (visualization)
find_library(CHRONO_IRRLICHT NAMES libChronoEngine_irrlicht.so REQUIRED)

# we depend on Eigen3 as well
find_package(Eigen3 REQUIRED)

# find where chrono's headers residee, which exists either in /usr/local/include or /usr/include
# or maybe somewhere else
find_path(CHRONO_INCLUDE NAMES "chrono/ChConfig.h" REQUIRED)
set(CHRONO_INCLUDE "${CHRONO_INCLUDE}/chrono") # Somewhat questionable

# same thing but irrlicht. Should be at the same place as chrono but its good to make sure
find_path(CHRONO_IRRLICHT_INCLUDE NAMES "irrlicht/irrlicht.h" REQUIRED)
set(CHRONO_IRRLICHT_INCLUDE "${CHRONO_IRRLICHT_INCLUDE}/irrlicht") # Somewhat questionable

# TODO EIGEN3_INCLUDE_DIR is not set tho
include_directories(${EIGEN3_INCLUDE_DIR} ${CHRONO_INCLUDE} ${CHRONO_IRRLICHT_INCLUDE})

# link libraries
link_libraries(${CHRONO_ENGINE} ${CHRONO_IRRLICHT})

# build our final executable
file( GLOB SOURCES src/*.cpp )
add_executable(ChainFountain ChainFountain.cpp ${SOURCES})
