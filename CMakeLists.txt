cmake_minimum_required (VERSION 2.8)
set(APP_NAME tkom)
set(APP_NAME_DEBUG tkom-debug)

project (${APP_NAME})

set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DEXIT_ON_FAIL")

include_directories(
  include
  include/tokens
  include/syntax_tree
)

set(APP_SRC
  src/main.cpp
  src/InputManager.cpp
  src/MessageHandler.cpp
  src/Interpreter.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/SemanticChecker.cpp
  src/Executor.cpp
  src/Library.cpp
  src/Variable.cpp
)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

add_executable(${APP_NAME} ${APP_SRC})



project (${APP_NAME_DEBUG})

set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_DEBUG -DEXIT_ON_FAIL")

include_directories(
  include
)

set(APP_SRC
  src/main.cpp
  src/InputManager.cpp
  src/MessageHandler.cpp
  src/Interpreter.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/SemanticChecker.cpp
  src/Executor.cpp
  src/Library.cpp
  src/Variable.cpp
)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

add_executable(${APP_NAME_DEBUG} ${APP_SRC})
