cmake_minimum_required(VERSION 3.5)

project(odysseus-kwin-plugins)

set(PROJECT_NAME "odysseus-kwin")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(plugins)

install(FILES config/kwinrc DESTINATION /etc/xdg)
