#!/bin/bash

# Создаем директорию сборки
BUILD_FOLDER="build"
SOURCE_FOLDER="src"  # Текущая директория, где CMakeLists.txt

if [ ! -d "$BUILD_FOLDER" ]; then
    mkdir "$BUILD_FOLDER"
fi

cd "$BUILD_FOLDER"

# Конфигурируем и собираем
cmake "../$SOURCE_FOLDER"
cmake --build . -j$(nproc)

cd ..
