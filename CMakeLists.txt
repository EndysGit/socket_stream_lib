cmake_minimum_required(VERSION 3.10)
    set(PROJECT_NAME sock_streams_lib)
    project(${PROJECT_NAME})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(SUB_DIRS thirdparty tests)

    # All internal headers
    set(INCLUDE_DIRS include)
    # ALL src SRC_FILES
    file(GLOB_RECURSE SRC_FILES src/*.cpp include/*)

    include_directories(${INCLUDE_DIRS})
    add_subdirectory(${SUB_DIRS})

    add_executable(${PROJECT_NAME} ${SRC_FILES} src/base_server.cpp include/base_server.h src/base_client.cpp include/base_client.h)
