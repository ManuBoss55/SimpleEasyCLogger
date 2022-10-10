cmake_minimum_required(VERSION 3.0)
project(SimpleEasyCLogger VERSION 0.1.0 DESCRIPTION "Configurator")
set(CMAKE_BUILD_TYPE Debug)

file(GLOB SIMPLEEASYCLOGGER_SOURCES src/*.c)
include_directories(inc)
add_library(SimpleEasyCLogger STATIC ${SIMPLEEASYCLOGGER_SOURCES})
target_link_libraries(SimpleEasyCLogger PRIVATE 
                            pthread
                            )

add_executable(Main "main.c")
target_include_directories(Main PUBLIC 
                            inc
                            )
target_link_libraries(Main PRIVATE 
                            SimpleEasyCLogger
                            )