cmake_minimum_required (VERSION 2.8)
set(APP_NAME TKOM)
project (${APP_NAME})

set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")

include_directories(
  include
)

set(APP_SRC
  src/main.cpp
  src/InputManager.cpp
  src/MessageHandler.cpp
  src/Interpreter.cpp
)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

add_executable(${APP_NAME} ${APP_SRC})
