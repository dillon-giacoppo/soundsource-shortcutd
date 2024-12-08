cmake_minimum_required(VERSION 3.30)
project(soundsource_shortcutd LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

if (NOT APPLE)
    message(FATAL_ERROR "${PROJECT_NAME} must be built on macos")
endif ()

## Configure
set(SOUNDSOURCE_SHORTCUT_NAME "Headphone EQ" CACHE STRING "set name of shortcut")


## Build

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "soundsource-shortcutd")

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        SOUNDSOURCE_SHORTCUT_NAME="${SOUNDSOURCE_SHORTCUT_NAME}"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreAudio")


## Install

# Install the daemon binary.
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Install the daemon plist.
set(daemon_plist "foo.dill.soundsource-shortcutd.plist")
set(daemon_destination "$ENV{HOME}/Library/LaunchAgents")

# Lazy initialize an install-time script to generate the plist file.
string(JOIN "\n" install_script
        "set(SOUNDSOURCE_SHORTCUT_NAME \"${SOUNDSOURCE_SHORTCUT_NAME}\")"
        "configure_file(\"${daemon_plist}.in\" \"${CMAKE_BINARY_DIR}/${daemon_plist}\" @ONLY)"
)
install(CODE "${install_script}")
install(FILES "${CMAKE_BINARY_DIR}/${daemon_plist}" DESTINATION "${daemon_destination}")

# Load plist.
install(CODE "execute_process(COMMAND launchctl load ${daemon_destination}/${daemon_plist})")
