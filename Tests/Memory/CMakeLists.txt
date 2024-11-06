add_executable(Memory main.cpp)

# vs项目放在Tests目录下
set_target_properties(Memory PROPERTIES FOLDER ${TestsFolderName})
