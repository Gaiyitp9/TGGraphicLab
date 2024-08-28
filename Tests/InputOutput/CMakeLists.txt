add_executable(InputOutput main.cpp)

# vs项目放在Tests目录下
set_target_properties(InputOutput PROPERTIES FOLDER ${TestsFolderName})
