cmake_minimum_required(VERSION 3.31)

project(xdg_shell_protocol VERSION 1.0)

# 使用wayland scanner生成xdg shell protocol的源文件
find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)
set(XDG_SHELL_XML "/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_HEADER "xdg-shell-client-protocol.h")
set(XDG_SHELL_SOURCE "xdg-shell-client-protocol.c")
add_custom_command(
    OUTPUT ${XDG_SHELL_HEADER}
    COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_XML} ${XDG_SHELL_HEADER}
    DEPENDS ${XDG_SHELL_XML}
    COMMENT "Generating xdg-shell-client-protocol.h"
)
add_custom_command(
    OUTPUT ${XDG_SHELL_SOURCE}
    COMMAND ${WAYLAND_SCANNER} private-code ${XDG_SHELL_XML} ${XDG_SHELL_SOURCE}
    DEPENDS ${XDG_SHELL_XML}
    COMMENT "Generating xdg-shell-client-protocol.c"
)

add_library(${PROJECT_NAME} STATIC ${XDG_SHELL_HEADER} ${XDG_SHELL_SOURCE})
add_library(TG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

include(GNUInstallDirs)

target_include_directories(${PROJECT_NAME} SYSTEM
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

include(TGInstall)
tg_install(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_INSTALL_INCLUDEDIR}
    ${CMAKE_INSTALL_LIBDIR}
    ${CMAKE_INSTALL_BINDIR}
)
