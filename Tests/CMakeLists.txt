# 设置测试目录名称
set(TestsFolderName Tests)

# 添加所有测试项目
add_subdirectory(DotNetHostRuntime)
add_subdirectory(InputOutput)
add_subdirectory(Math)
#list(APPEND AllTests Reflection)
add_subdirectory(Template)
add_subdirectory(WinSocketDemo)
