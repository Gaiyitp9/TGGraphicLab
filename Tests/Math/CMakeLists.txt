enable_testing()

find_package(GTest CONFIG REQUIRED)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES main.cpp)
add_executable(MathTest main.cpp)
# vs项目放在Tests目录下
set_target_properties(MathTest PROPERTIES FOLDER ${TestsFolderName})
target_link_libraries(MathTest PRIVATE GTest::gtest_main GraphicCore)

add_test(NAME AllTestsInMath COMMAND MathTest)
