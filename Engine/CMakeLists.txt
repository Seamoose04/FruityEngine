cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)

# GLAD
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# GLM
add_library(glm INTERFACE)
target_link_libraries(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

# stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/stb)

# JSON
add_library(json INTERFACE)
target_include_directories(json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/json)

# Sources
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
	src/*.cpp
	src/*.h
)

# Engine library
add_library(Engine STATIC ${ENGINE_SOURCES})

# Shaders
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(GLOB SHADERS "${SHADER_DIR}/*")
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_OUTPUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Copying shader files..."
)
foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    add_custom_command(
        TARGET copy_shaders
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SHADER}" "${SHADER_OUTPUT_DIR}/${SHADER_NAME}"
    )
endforeach()
add_dependencies(Engine copy_shaders)

# Include
find_package(OpenGL REQUIRED)
target_include_directories(Engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/glm
	${CMAKE_CURRENT_SOURCE_DIR}/external/stb
	${CMAKE_CURRENT_SOURCE_DIR}/external/json
)

# Link
target_link_libraries(Engine PUBLIC
	glfw
	glad
	stb
	OpenGL::GL
)

# Linux
if (UNIX AND NOT APPLE)
	target_link_libraries(Engine PRIVATE dl pthread X11)
endif()

# Warnings
# target_compile_options(Engine PRIVATE -Wall -Wextra -Wpedantic)