# 设置测试目录名称
set(TestsFolderName Tests)

# 集成googletest
include(FetchContent)
FetchContent_Declare(
        googletest
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/googletest"
        FIND_PACKAGE_ARGS NAMES GTest
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# 允许使用add_test()添加测试，生成适用于ctest的测试配置。
enable_testing()

# 添加所有测试项目
#add_subdirectory(DotNetHostRuntime)
add_subdirectory(InlineSpecifier)
add_subdirectory(InputOutput)
add_subdirectory(Math)
#add_subdirectory(QtDemo)
#add_subdirectory(Reflection)
add_subdirectory(Template)
add_subdirectory(WinSocketDemo)
add_subdirectory(Memory)
add_subdirectory(Delegate)
