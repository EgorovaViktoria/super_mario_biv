cmake_minimum_required(VERSION 3.16)

project(super_mario LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- 1. Собираем исходники ---
file(GLOB_RECURSE ENGINE_SOURCES 
    "controller/*.cpp"
    "levels/*.cpp"
    "model/*.cpp"
    "objects/*.cpp"
)

# Явно перечисляем Qt файлы
set(QT_UI_SOURCES
    "ui/qt/qt_ui_factory.cpp"
    "ui/qt/qt_game_map.cpp"
    "ui/qt/qt_ui_obj_rect_adapter.cpp"
    "ui/qt/ui_objects/qt_mario.cpp"
    "ui/qt/ui_objects/qt_enemy.cpp"
    "ui/qt/ui_objects/qt_flying_enemy.cpp"
    "ui/qt/ui_objects/qt_jumping_enemy.cpp"
    "ui/qt/ui_objects/qt_box.cpp"
    "ui/qt/ui_objects/qt_ship.cpp"
    "ui/qt/ui_objects/qt_full_box.cpp"
    "ui/qt/ui_objects/qt_money.cpp"
)

# Qt input handling for Unix
set(QT_INPUT_SOURCES
    "os_controls/unix/qt_input/qt_key_mapper.cpp"
    "os_controls/unix/qt_input/qt_init_settings.cpp"
)

set(MAIN_SOURCE "main.cpp")

add_executable(${PROJECT_NAME}
    ${MAIN_SOURCE}
    ${ENGINE_SOURCES}
    ${QT_UI_SOURCES}
    ${QT_INPUT_SOURCES}
)

# --- 2. ВАЖНО: Добавляем все папки в пути поиска заголовков ---
# Это позволяет писать #include "collisionable.hpp", даже если он лежит в model/
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/controller"
    "${CMAKE_CURRENT_SOURCE_DIR}/levels"
    "${CMAKE_CURRENT_SOURCE_DIR}/model"
    "${CMAKE_CURRENT_SOURCE_DIR}/objects"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/qt"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/qt/ui_objects"
    "${CMAKE_CURRENT_SOURCE_DIR}/os_controls"
    "${CMAKE_CURRENT_SOURCE_DIR}/os_controls/os_api"
    "${CMAKE_CURRENT_SOURCE_DIR}/os_controls/unix/qt_input"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
