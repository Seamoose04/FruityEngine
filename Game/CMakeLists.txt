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
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf  $<TARGET_FILE_DIR:Game>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Game/assets
        $<TARGET_FILE_DIR:Game>/assets
	COMMENT "Copying game assets..."
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} --build . --target copy_assets
)
