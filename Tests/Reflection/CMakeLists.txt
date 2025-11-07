#find_package(Clang REQUIRED CONFIG)
#
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#message(STATUS "LLVM include: ${LLVM_INCLUDE_DIRS}")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES main.cpp)

add_executable(Reflection main.cpp)
# vs项目放在Tests目录下
set_target_properties(Reflection PROPERTIES FOLDER ${TestsFolderName})

target_include_directories(Reflection PRIVATE ${LLVM_INCLUDE_DIRS} ../../RenderCore/Include)
#target_link_libraries(Reflection PRIVATE clangTooling)
target_link_libraries(Reflection PRIVATE GTest::gtest_main GraphicCore)

gtest_discover_tests(Reflection)
