cmake_minimum_required(VERSION 3.24)

if (DEFINED ENV{VCPKG_ROOT})
	set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE PATH "vcpkg root path")
else()
	set(VCPKG_ROOT "F:/vcpkg" CACHE PATH "vcpkg root path")
endif()
message(STATUS "vcpkg root path: ${VCPKG_ROOT}")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(TGGraphicLab)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	# msvc17.0以上
	if(MSVC_TOOLSET_VERSION GREATER_EQUAL 143)
		# 文件编码为utf-8，运行时char编码为utf-8，wchar_t编码为utf-16 LE
		add_compile_options(/utf-8 /arch:AVX2 /fp:precise)
	else()
		message(FATAL_ERROR "Invalid compiler")
	endif()
endif()

# 使用c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 开启使用文件夹选项，vs项目可以分类放在不同文件夹下
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(GraphicCore)
add_subdirectory(TGRenderer)
add_subdirectory(Tests)
add_subdirectory(RayTracing)
