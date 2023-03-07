cmake_minimum_required(VERSION 3.24)

project(TGRenderLab)

# msvc17.0以上
if(MSVC_VERSION GREATER_EQUAL 1930)
	# compile definitions unicode
	add_compile_definitions(UNICODE _UNICODE)
	# compile options utf-8 arch:avx2 fp:precise
	add_compile_options(/utf-8 /arch:AVX2 /fp:precise)
else()
	message(FATAL_ERROR "unsupported compiler")
endif()


# 使用c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(RenderCore)
add_subdirectory(TGRenderer)
