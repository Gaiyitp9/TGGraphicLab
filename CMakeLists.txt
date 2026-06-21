cmake_minimum_required(VERSION 3.31)

project(TGGraphicLab)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add_library是否默认编译成动态库
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# 将自定义模块目录添加到CMake的模块搜索路径中
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/BuildTools/CMake")

if(MSVC)
	set(TG_WINDOWS true)
	# 文件编码为utf-8，运行时char编码为utf-8，wchar_t编码为utf-16 LE
	# 禁用Spectre攻击的缓解措施警告warning C5045
	add_compile_options(/utf-8 /arch:AVX2 /fp:precise /Zc:__cplusplus /MP /W4 /wd5045)
	add_compile_definitions(TG_WINDOWS)
	# 强制所有目标使用动态链接CRT(/MD /MDd)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	# 开启使用文件夹选项，vs项目可以分类放在不同文件夹下
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if(LINUX)
	set(TG_LINUX true)
	add_compile_definitions(TG_LINUX)
	# 设置C++和C的默认符号可见性为隐藏
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)
	set(CMAKE_C_VISIBILITY_PRESET hidden)
	# 隐藏内联函数的符号，进一步减小体积
	set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
endif()

add_subdirectory(BuildTools/TGHeaderTool)
add_subdirectory(GraphicCore)
add_subdirectory(Tests)
add_subdirectory(TGRenderer)
