cmake_minimum_required(VERSION 3.10.0)
project(sim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
  set(CMAKE_CXX_COMPILER "clang++")
  include_directories("/usr/local/include/eigen3" "/usr/local/include/irrlicht" " /usr/local/include/chrono/collision/bullet/")
  link_libraries("/usr/local/lib/libChronoEngine.dylib" "/usr/local/lib/libChronoEngine_irrlicht.dylib")
else()
  set(CMAKE_CXX_COMPILER "g++")
  include_directories("/usr/include/eigen3" "/usr/include/irrlicht" " /usr/include/chrono/collision/bullet/")
  link_libraries("/usr/lib/libChronoEngine.so" "/usr/lib/libChronoEngine_irrlicht.so")
endif()

add_executable(sim sim.cpp)

