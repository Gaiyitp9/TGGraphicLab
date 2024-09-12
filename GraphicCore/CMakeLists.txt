file(GLOB_RECURSE HEADERS *.hpp *.h)
file(GLOB_RECURSE SOURCES *.cpp *.c)

list(FILTER HEADERS EXCLUDE REGEX Include/RHI)
list(FILTER SOURCES EXCLUDE REGEX Source/RHI)

if (NOT TG_PLATFORM_WINDOWS)
    list(FILTER HEADERS EXCLUDE REGEX Include/PAL/Windows)
    list(FILTER SOURCES EXCLUDE REGEX Source/PAL/Windows)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

set(TG_MATRIX_STORAGE_ORDER "RowMajor" CACHE STRING "Default matrix storage order" FORCE)
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")

add_library(GraphicCore ${HEADERS} ${SOURCES})
target_include_directories(GraphicCore PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Include)
target_link_libraries(GraphicCore PRIVATE dxgi d3d11 dxguid dbghelp PUBLIC PowerVR spdlog)
target_compile_definitions(GraphicCore
        PUBLIC $<$<STREQUAL:${TG_MATRIX_STORAGE_ORDER},RowMajor>:TG_ROW_MAJOR_MATRIX>
        # CRT库查找内存泄漏所需定义的宏
        $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:DEBUG>>:_CRTDBG_MAP_ALLOC>
)

install(TARGETS GraphicCore
        EXPORT GraphicCoreTargets
        RUNTIME DESTINATION GraphicOut/${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION GraphicOut/${CMAKE_INSTALL_LIBDIR}
)
