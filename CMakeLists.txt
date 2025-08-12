cmake_minimum_required(VERSION 3.10.0)
project(db_system)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "opt/homebrew/opt")
find_package (SQLite3 REQUIRED)
find_package (wxWidgets REQUIRED COMPONENTS core base)
find_package(Catch2 REQUIRED)
include(${wxWidgets_USE_FILE})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

set(MODEL_DIR "${SRC_DIR}/model")

add_executable(db_system
    ${SRC_DIR}/main.cpp

    # Model
    ${MODEL_DIR}/db_connection.cpp
    ${MODEL_DIR}/db_model.cpp

    # Test
    ${TEST_DIR}/test.cpp
    ${TEST_DIR}/db_connection_test.cpp
    ${TEST_DIR}/db_model_test.cpp
)
target_include_directories(db_system PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries (db_system SQLite::SQLite3 Catch2::Catch2 ${wxWidgets_LIBRARIES})