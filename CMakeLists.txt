cmake_minimum_required(VERSION 3.16)

project(SuperMarioBiv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Настройка Qt6 ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Создание исполняемого файла ---
# Добавляем файл main_qt.cpp как точку входа
add_executable(${PROJECT_NAME}
    src/main_qt.cpp
)

# --- Подключение исходников ---
# Вместо add_subdirectory (которые у тебя, видимо, пытаются привязаться к несуществующему таргету "Project"),
# мы подключим все папки напрямую, либо настроим их правильно.

# Чтобы не переписывать все вложенные CMakeLists.txt (которые выдают ошибки),
# проще всего собрать все исходники здесь.

# 1. UI (Qt)
target_sources(${PROJECT_NAME} PRIVATE
    src/ui/qt/qt_game_map.cpp
    src/ui/qt/qt_ui_factory.cpp
    # Включаем MOC (Meta-Object Compiler) для файлов с Q_OBJECT
    src/ui/qt/qt_game_map.hpp 
)

# 2. Логика игры (Model / Controller / Objects)
# Нужно перечислить все cpp файлы, которые используются в игре.
# Исключаем консольные UI файлы, так как мы собираем графическую версию.

file(GLOB_RECURSE SRC_FILES 
    "src/controller/*.cpp"
    "src/levels/*.cpp"
    "src/model/*.cpp"
    "src/objects/*.cpp"
    "src/os_controls/*.cpp" # Тут аккуратно, если там windows-специфичный код
)

# Исключаем main.cpp (консольный), так как у нас уже есть main_qt.cpp
list(FILTER SRC_FILES EXCLUDE REGEX "src/main.cpp")

# Исключаем Windows-специфичные файлы, так как ты на Ubuntu
list(FILTER SRC_FILES EXCLUDE REGEX "src/os_controls/windows/.*")

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})

# --- Подключение путей к заголовкам (Include Directories) ---
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/controller
    src/levels
    src/model
    src/objects
    src/os_controls
    src/ui/qt
    src/ui/qt/ui_objects
)

# --- Линковка библиотек ---
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# --- Настройка для Linux/Ubuntu (если нужно найти потоки) ---
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)