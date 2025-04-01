cmake_minimum_required(VERSION 3.30)

project(QtDemo)

set(QtLibraryPath "F:/Qt/6.8.3/msvc2022_64" CACHE STRING "Qt directory" FORCE)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${QtLibraryPath})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core OpenGLWidgets Widgets REQUIRED)

add_executable(QtDemo main.cpp OpenGLWidget.cpp OpenGLWidget.h)
target_link_libraries(QtDemo PRIVATE Qt6::Core Qt6::OpenGLWidgets Qt6::Widgets)

if (CMAKE_GENERATOR STREQUAL "Visual Studio 17 2022")
    set(WORKING_DIRECTORY $<TARGET_FILE_DIR:QtDemo/..)
elseif (CMAKE_GENERATOR STREQUAL "Ninja")
    set(WORKING_DIRECTORY $<TARGET_FILE_DIR:QtDemo>)
else()
    message(STATUS "Unknown generator: ${CMAKE_GENERATOR}")
endif()

set(SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/triangle.vert ${CMAKE_CURRENT_SOURCE_DIR}/triangle.frag)
add_custom_command(
        TARGET QtDemo
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${SHADERS} ${WORKING_DIRECTORY}
)
