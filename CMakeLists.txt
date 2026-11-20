cmake_minimum_required(VERSION 3.20)
project(NativeEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(native_engine
    src/Engine.cpp
    src/Renderer.cpp
    src/Window.cpp
)
target_include_directories(native_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(native_engine PUBLIC cxx_std_20)
if(MSVC)
    target_compile_options(native_engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(native_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(native_engine_demo src/main.cpp)
target_link_libraries(native_engine_demo PRIVATE native_engine)

enable_testing()
add_executable(native_engine_tests tests/EngineTests.cpp)
target_link_libraries(native_engine_tests PRIVATE native_engine)
add_test(NAME native_engine_tests COMMAND native_engine_tests)
