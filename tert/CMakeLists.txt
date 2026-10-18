cmake_minimum_required(VERSION 3.10)
project(OpenCVTest)

# Автоматически находим OpenCV по переменной OpenCV_DIR
find_package(OpenCV REQUIRED)

# Подключаем заголовочные файлы
include_directories(${OpenCV_INCLUDE_DIRS})

# Создаём исполняемый файл
add_executable(OpenCVTest main.cpp)

# Связываем библиотеки
target_link_libraries(OpenCVTest ${OpenCV_LIBS})