cmake_minimum_required(VERSION 3.31)

project(glad VERSION 2.0.8)
message(STATUS "glad version: ${PROJECT_VERSION}")

set(HEADERS Public/KHR/khrplatform.h Public/glad/gl.h)
set(SOURCES Private/gl.c)
if (TG_WINDOWS)
    list(APPEND HEADERS Public/glad/wgl.h)
    list(APPEND SOURCES Private/wgl.c)
endif()

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
add_library(TG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

include(GNUInstallDirs)

target_include_directories(${PROJECT_NAME} SYSTEM
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Private>
)

if (TG_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)
endif()

include(TGInstall)
tg_install(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_INSTALL_INCLUDEDIR}
    ${CMAKE_INSTALL_LIBDIR}
    ${CMAKE_INSTALL_BINDIR}
)
