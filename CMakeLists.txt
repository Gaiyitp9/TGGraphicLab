cmake_minimum_required(VERSION 3.24)

# 设置toolchain文件，使用vcpkg管理部分三方库
if (DEFINED ENV{VCPKG_ROOT})
	set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE PATH "vcpkg root path")
else()
	set(VCPKG_ROOT "F:/vcpkg" CACHE PATH "vcpkg root path")
endif()
message(STATUS "vcpkg root path: ${VCPKG_ROOT}")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(TGGraphicLab)
# 文件编码为utf-8，运行时char编码为utf-8，wchar_t编码为utf-16 LE
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>
		$<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
		$<$<CXX_COMPILER_ID:MSVC>:/fp:precise>)

# 使用c++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 开启使用文件夹选项，vs项目可以分类放在不同文件夹下
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 默认编译为动态库
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ${BUILD_SHARED_LIBS})

add_subdirectory(ThirdParty)
add_subdirectory(GraphicCore)
add_subdirectory(TGRenderer)
add_subdirectory(Tests)
add_subdirectory(RayTracing)
