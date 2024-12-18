add_executable(InlineSpecifier main.cpp file1.cpp)

# vs项目放在Tests目录下
set_target_properties(InlineSpecifier PROPERTIES FOLDER ${TestsFolderName})
