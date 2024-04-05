project (odysseusdecoration)
set(CMAKE_CXX_STANDARD 11)

find_package(KF5CoreAddons REQUIRED)
find_package(KF5Config REQUIRED)
find_package(KF5WindowSystem REQUIRED)
find_package(KDecoration2 REQUIRED)
find_package(Qt5 CONFIG REQUIRED COMPONENTS Gui Core)

# Find libodysseus-system
find_library(libodysseus-system NAMES libodysseus-system PATHS /usr/local/lib REQUIRED)

set (decoration_SRCS
    decoration.cpp
    button.cpp
    resources.qrc
)

add_library (odysseusdecoration MODULE
    ${decoration_SRCS}
)

target_link_libraries (odysseusdecoration
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::ConfigCore
        KF5::ConfigGui
        KF5::CoreAddons
        KF5::WindowSystem
        ${libodysseus-system}  # Link libodysseus-system

    PRIVATE
        KDecoration2::KDecoration
)

install (TARGETS odysseusdecoration
         DESTINATION ${QT_PLUGINS_DIR}/org.kde.kdecoration2)
