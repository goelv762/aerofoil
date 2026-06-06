cmake_minimum_required(VERSION 3.20)
project(simulation)

set(EXEC program)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(BASEPATH "${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE RENDERING src/rendering/*.cpp)
file(GLOB_RECURSE PHYSICS src/physics/*.cpp)
file(GLOB_RECURSE MATH src/math/*.cpp)

# Find SDL3 via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED sdl3-image)

add_library(rendering SHARED ${RENDERING})
add_library(physics SHARED ${PHYSICS})
add_library(math SHARED ${MATH})

target_include_directories(rendering PRIVATE ${SDL3_IMAGE_INCLUDE_DIRS})
target_link_libraries(rendering PRIVATE ${SDL3_IMAGE_LIBRARIES})

target_include_directories(physics PRIVATE ${SDL3_INCLUDE_DIRS})

set_target_properties(rendering PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
set_target_properties(physics PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
set_target_properties(math PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")

add_executable(${EXEC} src/main.cpp)

target_include_directories(${EXEC} PRIVATE ${SDL3_INCLUDE_DIRS} ${SDL3_IMAGE_INCLUDE_DIRS})
target_link_libraries(${EXEC} PRIVATE rendering physics math ${SDL3_LIBRARIES} ${SDL3_IMAGE_LIBRARIES})
target_compile_options(${EXEC} PRIVATE ${SDL3_CFLAGS_OTHER})
