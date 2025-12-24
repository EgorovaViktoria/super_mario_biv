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

# Явно перечисляем Qt файлы, чтобы ничего не потерять
set(QT_UI_SOURCES
    "ui/qt/qt_game_window.cpp"
    "ui/qt/qt_ui_factory.cpp"
    "ui/qt/qt_game_map.cpp"  # Вот этот файл, который мы сейчас создадим
)

set(MAIN_SOURCE "main.cpp")

add_executable(${PROJECT_NAME}
    ${MAIN_SOURCE}
    ${ENGINE_SOURCES}
    ${QT_UI_SOURCES}
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
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
