cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all editor source files
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.h
)

# Build the editor executable
add_executable(Editor ${EDITOR_SOURCES})

# Include engine headers
target_include_directories(Editor PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine/src
)

# Link engine library
target_link_libraries(Editor PRIVATE Engine)

if (WIN32)
    target_link_options(Editor PRIVATE /WHOLEARCHIVE:$<TARGET_FILE:Engine>)
else()
    target_link_options(Editor PRIVATE
        -Wl,--whole-archive
        $<TARGET_FILE:Engine>
        -Wl,--no-whole-archive
    )
endif()

# Copy assets
add_custom_target(CopyEditorAssets ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf $<TARGET_FILE_DIR:Editor>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Editor/assets
        $<TARGET_FILE_DIR:Editor>/assets
    COMMENT "Copying editor assets..."
    DEPENDS Editor
)
