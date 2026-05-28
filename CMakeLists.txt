cmake_minimum_required(VERSION 3.20)
project(simulation)

set(EXEC program)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(BASEPATH "${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE RENDERING src/rendering/*.cpp)
file(GLOB_RECURSE PHYSICS src/physics/*.cpp)

# Find SDL3 via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

add_library(rendering SHARED ${RENDERING})
add_library(physics SHARED ${PHYSICS})

set_target_properties(rendering PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
set_target_properties(physics PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")

add_executable(${EXEC} src/main.cpp)

target_include_directories(${EXEC} PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(${EXEC} PRIVATE rendering physics ${SDL3_LIBRARIES})
target_compile_options(${EXEC} PRIVATE ${SDL3_CFLAGS_OTHER})
