cmake_minimum_required(VERSION 3.22)
set(PROJECT_NAME SECLogger_PROJECT)
set(PROJECT_VERSION 0.1.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

set(MAIN_FOLDER_PROJECT SimpleEasyCLogger)
# if(TARGET_GROUP STREQUAL Test)
#     set(MAIN_FOLDER_PROJECT test)
# endif()
# include(${PROJECT_SOURCE_DIR}/../dependencies/dependencies_defs.cmake)

# add_subdirectory(${JSON_DIR} ${JSON_DIR}/build)
# add_subdirectory(${DBMNG_DIR} ${DBMNG_DIR}/build)
# add_subdirectory(${COMM_SYS_DIR} ${COMM_SYS_DIR}/build)

# set(BUILD_TESTS OFF CACHE BOOL "Not building unit tests for ZMQ and libconfig")
# add_subdirectory(${ZMQ_DIR} ${ZMQ_DIR}/build)
# add_subdirectory(${LIBCONFIGPP_DIR} ${LIBCONFIGPP_DIR}/build)

# set(ZMQPP_LIBZMQ_CMAKE true CACHE BOOL "libzmq is build through cmake too" )
# set(ZEROMQ_LIB_DIR ${ZMQ_DIR}/build/lib CACHE PATH "ZeroMQ library directory")
# set(ZEROMQ_INCLUDE_DIR ${ZMQ_DIR}/include CACHE PATH "ZeroMQ include directory")
# add_subdirectory(${ZMQPP_DIR} ${ZMQPP_DIR}/build)

if((TARGET_GROUP STREQUAL Debug) OR (TARGET_GROUP STREQUAL All)) 

    add_subdirectory(SimpleEasyCLogger)
    # add_subdirectory(mock)

endif()

if((TARGET_GROUP STREQUAL Test) OR (TARGET_GROUP STREQUAL All)) 
    include(CTest)  

    add_definitions(-DUNITY_OUTPUT_COLOR)
    add_definitions(-DUNITY_INCLUDE_DOUBLE)
    add_subdirectory(${UNITY_DIR}/.. ${UNITY_DIR}/../build)
    
    add_subdirectory(test)  
    
endif()

if(NOT (TARGET_GROUP STREQUAL Debug) AND NOT (TARGET_GROUP STREQUAL Test) AND NOT (TARGET_GROUP STREQUAL All)) 
    message(FATAL_ERROR "Uknown TARGET_GROUP!") 
endif()