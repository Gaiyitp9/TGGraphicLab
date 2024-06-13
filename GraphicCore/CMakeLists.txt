file(GLOB_RECURSE HEADERS *.hpp *.h)
file(GLOB_RECURSE SOURCES *.cpp *.c)

list(FILTER HEADERS EXCLUDE REGEX Include/Graphics)
list(FILTER SOURCES EXCLUDE REGEX Source/Graphics)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(FILTER HEADERS EXCLUDE REGEX Include/PAL/Windows)
    list(FILTER SOURCES EXCLUDE REGEX Source/PAL/Windows)
endif()

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${HEADERS} ${SOURCES})

set(TG_MATRIX_STORAGE_ORDER "RowMajor" CACHE STRING "Default matrix storage order" FORCE)
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")

find_package(spdlog CONFIG REQUIRED)

add_library(RenderCore STATIC ${HEADERS} ${SOURCES})
target_include_directories(RenderCore PUBLIC Include)
target_link_libraries(RenderCore PRIVATE dxgi d3d11 dxguid dbghelp opengl32 PUBLIC spdlog::spdlog glad)
target_compile_definitions(RenderCore
        PUBLIC $<$<STREQUAL:${TG_MATRIX_STORAGE_ORDER},RowMajor>:TG_ROW_MAJOR_MATRIX>
        # CRT库查找内存泄漏所需定义的宏
        $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:DEBUG>>:_CRTDBG_MAP_ALLOC>
)
