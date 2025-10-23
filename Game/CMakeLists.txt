cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all game source files
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.h
)

# Build the game executable
add_executable(Game ${GAME_SOURCES})

# Include engine headers
target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine/src
)

# Link engine library
target_link_libraries(Game PRIVATE Engine)

if (WIN32)
    target_link_options(Game PRIVATE /WHOLEARCHIVE:$<TARGET_FILE:Engine>)
else()
    target_link_options(Game PRIVATE
        -Wl,--whole-archive
        $<TARGET_FILE:Engine>
        -Wl,--no-whole-archive
    )
endif()

# Copy assets
file(GLOB_RECURSE ASSETS
    RELATIVE "${CMAKE_SOURCE_DIR}/Game/assets"
    "${CMAKE_SOURCE_DIR}/Game/assets/*"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Game>/assets
    COMMAND ${CMAKE_COMMAND} -E echo "Copying asset files..."
)
foreach(ASSET ${ASSETS})
    get_filename_component(ASSET_DIR "${ASSET}" PATH)
    add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Game>/assets/${ASSET_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Game/assets/${ASSET}"
            "$<TARGET_FILE_DIR:Game>/assets/${ASSET}"
    )
endforeach()