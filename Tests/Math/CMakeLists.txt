source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES main.cpp)
add_executable(Math main.cpp)
# vs项目放在Tests目录下
set_target_properties(Math PROPERTIES FOLDER ${TestsFolderName})
target_include_directories(Math PRIVATE ../../RenderCore/Include)
target_compile_definitions(Math PRIVATE -DTG_ROW_MAJOR_MATRIX)
