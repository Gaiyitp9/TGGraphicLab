source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES main.cpp ExpressionTemplate.hpp ReflectionTest.hpp)
add_executable(Template main.cpp ExpressionTemplate.hpp ReflectionTest.hpp)
# vs项目放在Tests目录下
set_target_properties(Template PROPERTIES FOLDER ${TestsFolderName})
target_include_directories(Template PRIVATE ../../GraphicCore/Public)
