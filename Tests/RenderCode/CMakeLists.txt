source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES main.cpp)

add_executable(RenderCode main.cpp)
# vs项目放在Tests目录下
set_target_properties(RenderCode PROPERTIES FOLDER ${TestsFolderName})
target_include_directories(RenderCode PRIVATE ../../ThirdParty)