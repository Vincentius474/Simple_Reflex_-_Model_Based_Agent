cmake_minimum_required(VERSION 3.20)
project(simple_reflex_agent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive- /Zc:__cplusplus)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(agent_lib INTERFACE)
target_include_directories(agent_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Simulator
add_executable(vacuum_agent src/main.cpp)
target_link_libraries(vacuum_agent PRIVATE agent_lib)

# Tests
enable_testing()
add_executable(agent_tests tests/test_agents.cpp)
target_link_libraries(agent_tests PRIVATE agent_lib)
add_test(NAME agent_tests COMMAND agent_tests)