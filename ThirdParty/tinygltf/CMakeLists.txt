cmake_minimum_required(VERSION 3.31)

project(tiny_gltf VERSION 2.9.7)
message(STATUS "tiny_gltf version: ${PROJECT_VERSION}")

FetchContent_Declare(
    nlohmann_json
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/nlohmannjson"
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    stb
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/stb"
)
FetchContent_MakeAvailable(stb)

file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
add_library(TG::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

include(GNUInstallDirs)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
    TINYGLTF_NO_INCLUDE_JSON
    TINYGLTF_NO_INCLUDE_RAPIDJSON
    TINYGLTF_NO_INCLUDE_STB_IMAGE
    TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
)
target_include_directories(${PROJECT_NAME} SYSTEM
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Private>
)
target_link_libraries(${PROJECT_NAME} PUBLIC TG::nlohmann_json TG::stb)

include(TGInstall)
tg_install(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_INSTALL_INCLUDEDIR}
    ${CMAKE_INSTALL_LIBDIR}
    ${CMAKE_INSTALL_BINDIR}
)
