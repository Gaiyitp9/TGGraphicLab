enable_testing()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES main.cpp)

find_package(GTest REQUIRED)
add_executable(MathTest main.cpp)
# vs项目放在Tests目录下
set_target_properties(MathTest PROPERTIES FOLDER ${TestsFolderName})
target_link_libraries(MathTest PRIVATE GTest::gtest_main GraphicCore)

include(GoogleTest)
gtest_discover_tests(MathTest)
