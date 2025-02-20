source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES main.cpp)

add_executable(MathTest main.cpp)
target_link_libraries(MathTest PRIVATE GTest::gtest_main GraphicCore)

# vs项目放在Tests目录下
set_target_properties(MathTest PROPERTIES FOLDER ${TestsFolderName})

gtest_discover_tests(MathTest)
