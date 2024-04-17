cmake_minimum_required(VERSION 3.24)

project(TGGraphicLab)

# msvc17.0以上
if(MSVC_TOOLSET_VERSION GREATER_EQUAL 143)
	# 文件编码为utf-8，运行时char编码为utf-8，wchar_t编码为utf-16 LE
	add_compile_options(/utf-8 /arch:AVX2 /fp:precise)
else()
	message(FATAL_ERROR "unsupported compiler")
endif()

# 使用c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 开启使用文件夹选项，vs项目可以分类放在不同文件夹下
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(RenderCore)
add_subdirectory(TGRenderer)
add_subdirectory(Tests)
add_subdirectory(RayTracing)