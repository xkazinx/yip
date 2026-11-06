# yip for a web browser (WebAssembly, Emscripten). Windows builds it from yip.vcxproj
# (build.ps1); this file is the web one, run by build_web.ps1 against kazilib's SFML
# web build (kazilib web/build_deps.ps1):
#   emcmake cmake -S src/yip -B <build> -G Ninja -DSFML_STATIC_LIBRARIES=ON -DSFML_DIR=...
# Output: index.html / .js / .wasm / .data (the font): kazin.dev/projects/yip.
cmake_minimum_required(VERSION 3.21)
project(yip_web CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(YIP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 REQUIRED COMPONENTS Graphics)
include(${YIP_ROOT}/src/dep/kazilib/web/kazilib_web.cmake)

file(GLOB YIP_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(FILTER YIP_SOURCES EXCLUDE REGEX "stdafx\\.cpp$")

add_executable(yip ${YIP_SOURCES})
kazilib_web_target(yip)
target_include_directories(yip PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(yip PRIVATE SFML::Graphics)
target_link_options(yip PRIVATE "--preload-file=${YIP_ROOT}/deploy/arial.ttf@/arial.ttf")
