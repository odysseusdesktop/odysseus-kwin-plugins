cmake_minimum_required(VERSION 3.5)

project(cyber-kwin-plugins)

set(PROJECT_NAME "cyber-kwin")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(plugins)

install(FILES config/kwinrc DESTINATION /etc/xdg)
install(DIRECTORY scripts/cyberlauncher DESTINATION /usr/share/kwin/scripts)
install(DIRECTORY scripts/cyber_squash DESTINATION /usr/share/kwin/effects)
install(DIRECTORY tabbox/cyber_thumbnail DESTINATION /usr/share/kwin/tabbox)
