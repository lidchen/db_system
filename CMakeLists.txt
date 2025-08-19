cmake_minimum_required(VERSION 3.10.0)
project(db_system)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.*")

find_package (SQLite3 REQUIRED)
find_package (wxWidgets REQUIRED COMPONENTS core base)
find_package(Catch2 REQUIRED)
include(${wxWidgets_USE_FILE})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(MODEL_DIR "${SRC_DIR}/model")
set(VIEW_DIR "${SRC_DIR}/view")
set(CONTROLLER_DIR "${SRC_DIR}/controller")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

# Optionally toggle feature macros via cache variables for easy switching
option(ENABLE_DEBUG "Enable DEBUG_ON macro" ON)
option(ENABLE_UI "Enable UI_ON macro" ON)
option(ENABLE_TEST "Enable TEST_ON macro" OFF)

if(ENABLE_DEBUG)
    add_compile_definitions(DEBUG_ON)
else()
    add_compile_definitions(DEBUG_OFF)
endif()

if(ENABLE_TEST)
    add_compile_definitions(TEST_ON)
else()
    add_compile_definitions(TEST_OFF)
endif()

if(ENABLE_UI)
    add_compile_definitions(UI_ON)
else()
    add_compile_definitions(UI_OFF)
endif()

add_executable(db_system
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SRC_DIR}

    ${SRC_DIR}/main.cpp

    # Model
    ${MODEL_DIR}/db_connection.cpp
    ${MODEL_DIR}/db_model.cpp

    # Controller
    ${CONTROLLER_DIR}/db_controller.cpp

    # View
    ${VIEW_DIR}/db_view.cpp
    # ${VIEW_DIR}/welcome_panel.cpp
    ${VIEW_DIR}/main_frame.cpp
    ${VIEW_DIR}/gui_components/listbox.cpp
    ${VIEW_DIR}/gui_components/labeled_text_ctrl.cpp
    ${VIEW_DIR}/dialog/wx_base_dialog.cpp

    # Event

    # Test
    ${TEST_DIR}/test.cpp
    ${TEST_DIR}/db_connection_test.cpp
    ${TEST_DIR}/db_model_test.cpp
)
target_include_directories(db_system PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries (db_system SQLite::SQLite3 Catch2::Catch2 ${wxWidgets_LIBRARIES})