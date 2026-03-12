cmake_minimum_required(VERSION 3.16)
project(Pinball VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML (requires SFML 2.x)
find_package(SFML 2.5 COMPONENTS graphics window audio system REQUIRED)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Ball.cpp
    src/Flipper.cpp
    src/Physics.cpp
    src/UI.cpp
)

add_executable(pinball ${SOURCES})

target_include_directories(pinball PRIVATE src)

target_link_libraries(pinball
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-system
)

# Copy assets directory next to the executable after build
add_custom_command(TARGET pinball POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:pinball>/assets
    COMMENT "Copying assets to build directory"
)
