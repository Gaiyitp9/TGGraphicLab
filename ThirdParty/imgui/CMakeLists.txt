cmake_minimum_required(VERSION 3.31)

project(ImGui VERSION 1.92.7)
message(STATUS "ImGui version: ${PROJECT_VERSION}")

find_package(Vulkan REQUIRED)

FetchContent_Declare(
    glad
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/glad"
)
FetchContent_MakeAvailable(glad)

file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)

if (NOT TG_WINDOWS)
    list(FILTER HEADERS EXCLUDE REGEX "Public/imgui_impl_win32.h")
    list(FILTER SOURCES EXCLUDE REGEX "Private/imgui_impl_win32.cpp")
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
target_link_libraries(${PROJECT_NAME} PUBLIC TG::glad Vulkan::Vulkan)

include(TGInstall)
tg_install(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_INSTALL_INCLUDEDIR}
    ${CMAKE_INSTALL_LIBDIR}
    ${CMAKE_INSTALL_BINDIR}
)
