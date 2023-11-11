cmake_minimum_required(VERSION 3.18)

project(state_minimization)

set(SRC_DIR src)
set(INCLUDE_DIR include)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} ${SRC_DIR}/B11030202.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})