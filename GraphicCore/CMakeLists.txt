set(LIBRARY_NAME GraphicCore)

find_package(mimalloc 3.1.5 REQUIRED)
find_package(spdlog 1.15.1 REQUIRED)
find_package(glm REQUIRED)

include(FetchContent)

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

file(GLOB_RECURSE HEADERS *.hpp *.h)
file(GLOB_RECURSE SOURCES *.cpp *.c)

if (NOT TG_WINDOWS)
    list(FILTER HEADERS EXCLUDE REGEX "Public(/.*)+/Windows")
    list(FILTER SOURCES EXCLUDE REGEX "Private(/.*)+/Windows")
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
target_include_directories(${LIBRARY_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Private
)
target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    mimalloc spdlog::spdlog glm::glm
    TG::ImGui TG::tiny_gltf
)

set_target_properties(${LIBRARY_NAME} PROPERTIES DEBUG_POSTFIX d)
