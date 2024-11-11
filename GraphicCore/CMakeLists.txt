set(PROJECT_NAME GraphicCore)

file(GLOB_RECURSE HEADERS *.hpp *.h)
file(GLOB_RECURSE SOURCES *.cpp *.c)
list(FILTER HEADERS EXCLUDE REGEX Include/RHI)
list(FILTER SOURCES EXCLUDE REGEX Source/RHI)

if (NOT TG_WINDOWS)
    # 剔除Windows目录下的代码
    list(FILTER HEADERS EXCLUDE REGEX "Include(/.*)+/Windows")
    list(FILTER SOURCES EXCLUDE REGEX "Source(/.*)+/Windows")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})


set(TG_MATRIX_STORAGE_ORDER "RowMajor" CACHE STRING "Default matrix storage order" FORCE)
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Include)
target_link_libraries(${PROJECT_NAME} PRIVATE dxgi d3d11 dxguid dbghelp PUBLIC PowerVR spdlog ImGui)
target_compile_definitions(${PROJECT_NAME}
        PUBLIC $<$<STREQUAL:${TG_MATRIX_STORAGE_ORDER},RowMajor>:TG_ROW_MAJOR_MATRIX>
        # CRT库查找内存泄漏所需定义的宏
        PRIVATE $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:DEBUG>>:_CRTDBG_MAP_ALLOC>
)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)
