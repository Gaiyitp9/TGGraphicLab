include(FetchContent)
# 下载并集成mimalloc
FetchContent_Declare(
        mimalloc
        GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
        GIT_TAG        e14cfd2578fd68205b8d38895abe120bb622580e     # v3.0.1
        GIT_PROGRESS YES
)
FetchContent_MakeAvailable(mimalloc)
# 下载并集成spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        f355b3d58f7067eee1706ff3c801c2361011f3d5     # v1.15.1
        GIT_PROGRESS YES
)
FetchContent_MakeAvailable(spdlog)
# 集成ImGui
set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/imgui")
FetchContent_Declare(
        ImGui
        SOURCE_DIR ${IMGUI_PATH}
)
FetchContent_MakeAvailable(ImGui)
# 集成stb
set(STB_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/stb")
FetchContent_Declare(
        stb
        SOURCE_DIR ${STB_PATH}
)
FetchContent_MakeAvailable(stb)

set(LIBRARY_NAME "GraphicCore")

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
target_link_libraries(${LIBRARY_NAME} PUBLIC spdlog::spdlog mimalloc TG::ImGui TG::stb)
target_compile_definitions(${LIBRARY_NAME}
        PUBLIC $<$<STREQUAL:${TG_MATRIX_STORAGE_ORDER},RowMajor>:TG_ROW_MAJOR_MATRIX>
        # CRT库查找内存泄漏所需定义的宏
        $<$<AND:$<PLATFORM_ID:Windows>,$<CONFIG:DEBUG>>:_CRTDBG_MAP_ALLOC>
)
set_target_properties(${LIBRARY_NAME} PROPERTIES DEBUG_POSTFIX d)
