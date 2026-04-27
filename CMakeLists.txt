cmake_minimum_required(VERSION 3.20)
project(fekt-mpg)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_library(GLUT_LIBRARY GLUT REQUIRED)
find_package(GLEW REQUIRED)

# Suppress macOS OpenGL deprecation warnings
add_compile_definitions(GL_SILENCE_DEPRECATION)

# Auto-discover all .cpp files and create a separate executable for each
file(GLOB CPP_SOURCES "*.cpp")

foreach(SOURCE ${CPP_SOURCES})
    get_filename_component(EXEC_NAME ${SOURCE} NAME_WE)
    add_executable(${EXEC_NAME} ${SOURCE})
    target_link_libraries(${EXEC_NAME} OpenGL::GL ${GLUT_LIBRARY} GLEW::GLEW)
endforeach()