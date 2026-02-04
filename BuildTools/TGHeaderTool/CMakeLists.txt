find_package(Clang CONFIG)

if (NOT Clang_FOUND)
    message(STATUS "TG Header Tool project disabled")
    return()
endif()

message(STATUS "TG Header Tool project enabled")

set(EXECUTABLE_NAME "TGHeaderTool")

file(GLOB_RECURSE SOURCES *.cpp *.c)
file(GLOB_RECURSE HEADERS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${HEADERS} ${SOURCES})

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Private)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE clangTooling)
