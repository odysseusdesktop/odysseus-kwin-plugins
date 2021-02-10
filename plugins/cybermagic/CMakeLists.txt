set(TARGET "cybermagic")
set(CMAKE_AUTOMOC ON)

find_package(KF5Config REQUIRED)

add_library(
    ${TARGET} MODULE
    cybermagic.cpp
)

target_link_libraries(
    ${TARGET}
    PUBLIC
    Qt5::Core
    Qt5::Gui
    PRIVATE
    KF5::CoreAddons
    KF5::WindowSystem
    KF5::ConfigCore
)

kcoreaddons_desktop_to_json(cybermagic cybermagic.desktop
    SERVICE_TYPES kwineffect.desktop
)

install(TARGETS ${TARGET} DESTINATION "${QT_PLUGINS_DIR}/kwin/effects/plugins")
