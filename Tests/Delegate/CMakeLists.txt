add_executable(Delegate main.cpp)
target_link_libraries(Delegate PRIVATE GraphicCore)
# vs项目放在Tests目录下
set_target_properties(Delegate PROPERTIES FOLDER ${TestsFolderName})
