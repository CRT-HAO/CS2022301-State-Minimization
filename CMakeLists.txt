cmake_minimum_required(VERSION 3.18)

project(state_minimization)

set(SRC_DIR src)
set(INCLUDE_DIR include)

set(CMAKE_CXX_STANDARD 14)

# set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
add_executable(${PROJECT_NAME}
  ${SRC_DIR}/B11030202.cpp
  ${SRC_DIR}/state_minimization.cpp
  ${SRC_DIR}/kiss.cpp
  ${SRC_DIR}/dot.cpp
  ${SRC_DIR}/debug.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-sNO_DISABLE_EXCEPTION_CATCHING")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-sNO_DISABLE_EXCEPTION_CATCHING -sEXTRA_EXPORTED_RUNTIME_METHODS='[\"cwrap\", \"allocate\", \"intArrayFromString\", \"Pointer_stringify\"]' -sEXPORT_ES6=1  -sMODULARIZE=1 --no-entry")