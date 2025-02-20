set(LIBRARY_NAME GraphicCore)

find_package(ImGui 1.91.4 REQUIRED)
find_package(mimalloc 2.1.7 REQUIRED)
find_package(PowerVR 5.13 REQUIRED)
find_package(spdlog 1.15.0 REQUIRED)
find_package(stb 1.0 REQUIRED)
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE HEADERS *.hpp *.h)
file(GLOB_RECURSE SOURCES *.cpp *.c)

if (NOT TG_WINDOWS)
    # 剔除Windows目录下的代码
    list(FILTER HEADERS EXCLUDE REGEX "Public(/.*)+/Windows")
    list(FILTER SOURCES EXCLUDE REGEX "Private(/.*)+/Windows")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

set(TG_MATRIX_STORAGE_ORDER "RowMajor" CACHE STRING "Default matrix storage order" FORCE)
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")

add_library(${LIBRARY_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Private)
target_link_libraries(${LIBRARY_NAME} PUBLIC  TG::ImGui TG::PowerVR spdlog::spdlog TG::stb
        mimalloc Vulkan::Vulkan)
target_compile_definitions(${LIBRARY_NAME}
        PUBLIC $<$<STREQUAL:${TG_MATRIX_STORAGE_ORDER},RowMajor>:TG_ROW_MAJOR_MATRIX>
        # CRT库查找内存泄漏所需定义的宏
        $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:DEBUG>>:_CRTDBG_MAP_ALLOC>
)
set_target_properties(${LIBRARY_NAME} PROPERTIES DEBUG_POSTFIX d)
