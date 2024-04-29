set(TG_MATRIX_STORAGE_ORDER "RowMajor" CACHE STRING "Default matrix storage order" FORCE)
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")

file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE INCLUDES *.hpp *.h *.inl)
#list(APPEND INCLUDES )
#list(APPEND SOURCES
#        Source/Chronometer.cpp
#        Source/Utility.cpp
#        Source/Diagnostics/)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCLUDES})

add_library(RenderCore STATIC ${SRCS} ${INCS})
target_include_directories(RenderCore PUBLIC Include ThirdParty)
target_link_libraries(RenderCore PRIVATE dxgi d3d11 dxguid dbghelp opengl32)

if (TG_MATRIX_STORAGE_ORDER MATCHES "RowMajor")
    target_compile_definitions(RenderCore PRIVATE -DTG_ROW_MAJOR_MATRIX)
endif()

message(STATUS "Platform: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "Source files: ${INCLUDES}")
