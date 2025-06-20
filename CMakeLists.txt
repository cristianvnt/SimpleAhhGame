cmake_minimum_required(VERSION 3.28)
project(SimpleAhhGame C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/third_party)

# Declare dependencies

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/glfw
)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/glm
)

# Third party
# stb_image.h
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/third_party/stb
)

FetchContent_MakeAvailable(glfw glm stb)

# GLAD validation
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)
    message(WARNING "GLAD not found. Download from https://glad.dav1d.de (API: Core, Profile: Core, Version: 4.6)")
    message(FATAL_ERROR "GLAD required in libs/glad/")
endif()

# Library targets
# GLAD (local loader)
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/libs/glad/include)

# stb
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

# Sources and executable
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw
    glad
    glm
    stb_image
    OpenGL::GL
)

# On linux
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread X11)
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()

# Runtime resources
file(COPY
    ${CMAKE_SOURCE_DIR}/resources
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)