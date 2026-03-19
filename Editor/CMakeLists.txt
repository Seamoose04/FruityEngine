cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ImGui sources
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/Editor/external/imgui)
set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Collect all editor source files
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.h
)

# Build the editor executable
add_executable(Editor ${EDITOR_SOURCES} ${IMGUI_SOURCES})

# Include engine headers
target_include_directories(Editor PRIVATE
	${CMAKE_SOURCE_DIR}/Editor/src
	${IMGUI_DIR}
	${IMGUI_DIR}/backends
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
        ${CMAKE_SOURCE_DIR}/Game/assets
        $<TARGET_FILE_DIR:Editor>/assets
    COMMENT "Copying assets for editor..."
    DEPENDS Editor
)
