find_package(Clang CONFIG)

if (NOT Clang_FOUND)
    message(STATUS "Clang not found. Reflection test disabled")
    return()
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Found Clang ${Clang_VERSION}")
message(STATUS "LLVM include: ${LLVM_INCLUDE_DIRS}")

file(GLOB_RECURSE HEADERS *.hpp *.h)
file(GLOB_RECURSE SOURCES *.cpp *.c)

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

add_executable(Reflection ${HEADERS} ${SOURCES})
# vs项目放在Tests目录下
set_target_properties(Reflection PROPERTIES FOLDER ${TestsFolderName})

target_link_libraries(Reflection PRIVATE clangTooling GTest::gtest_main GraphicCore)

gtest_discover_tests(Reflection)
