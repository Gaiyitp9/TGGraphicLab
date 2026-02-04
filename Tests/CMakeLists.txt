# 设置测试目录名称
set(TestsFolderName Tests)

find_package(GTest 1.16.0 REQUIRED)
include(GoogleTest)

# 允许使用add_test()添加测试，生成适用于ctest的测试配置。
enable_testing()

# 添加所有测试项目
#add_subdirectory(DotNetHostRuntime)
add_subdirectory(InlineSpecifier)
add_subdirectory(InputOutput)
add_subdirectory(Math)
#add_subdirectory(QtDemo)
add_subdirectory(Template)
add_subdirectory(WinSocketDemo)
add_subdirectory(Memory)
add_subdirectory(Delegate)
add_subdirectory(Reflection)
