set(LIBRARY_NAME GraphicCore)

include(FetchContent)

# 收集三方库
FetchContent_Declare(
    mimalloc
    GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
    GIT_TAG v3.2.8
    GIT_SHALLOW TRUE
)
set(MI_BUILD_TESTS OFF)
#set(MI_TRACK_ETW ON)
FetchContent_MakeAvailable(mimalloc)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
    GIT_SHALLOW TRUE
)
#set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_INSTALL ON)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)
set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    ImGui
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/imgui"
)
FetchContent_MakeAvailable(ImGui)

FetchContent_Declare(
    tiny_gltf
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/tinygltf"
)
FetchContent_MakeAvailable(tiny_gltf)
# Linux平台三方库
if (TG_LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(Wayland REQUIRED IMPORTED_TARGET wayland-client wayland-egl)
    pkg_check_modules(EGL REQUIRED IMPORTED_TARGET egl)

    FetchContent_Declare(
        xdg_shell_protocol
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/xdg_shell_protocol"
    )
    FetchContent_MakeAvailable(xdg_shell_protocol)
endif()

# 收集源文件
file(GLOB_RECURSE HEADERS *.hpp *.h)
file(GLOB_RECURSE SOURCES *.cpp *.c)
# 针对不同平台调整源文件
if (NOT TG_WINDOWS)
    list(FILTER HEADERS EXCLUDE REGEX "Public(/.*)+/Windows")
    list(FILTER SOURCES EXCLUDE REGEX "Private(/.*)+/Windows")
endif()
if (NOT TG_LINUX)
    list(FILTER HEADERS EXCLUDE REGEX "Public(/.*)+/Linux")
    list(FILTER SOURCES EXCLUDE REGEX "Private(/.*)+/Linux")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

set(TG_MATRIX_STORAGE_ORDER "ColumnMajor" CACHE STRING "Default matrix storage order")
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")

add_library(${LIBRARY_NAME} ${HEADERS} ${SOURCES})

target_compile_definitions(${LIBRARY_NAME}
    PUBLIC
    $<$<STREQUAL:${TG_MATRIX_STORAGE_ORDER},RowMajor>:TG_ROW_MAJOR_MATRIX>
    # CRT库查找内存泄漏所需定义的宏
    $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:DEBUG>>:_CRTDBG_MAP_ALLOC>
)

include(GNUInstallDirs)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Private>
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    mimalloc spdlog::spdlog glm::glm
    TG::ImGui TG::tiny_gltf
)
if(TG_LINUX)
    # gcc需要链接一个stdc++exp辅助库来编译stacktrace头文件
    target_link_libraries(${LIBRARY_NAME} PUBLIC stdc++exp PkgConfig::Wayland PkgConfig::EGL TG::xdg_shell_protocol)
endif()

set_target_properties(${LIBRARY_NAME} PROPERTIES DEBUG_POSTFIX d)
