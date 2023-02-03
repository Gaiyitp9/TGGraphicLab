cmake_minimum_required(VERSION 3.24)

project(TGRenderLab)

# only msvc 16.10 and newer support c++20
if(MSVC_VERSION GREATER_EQUAL 1930)
	# compile definitions unicode
	add_compile_definitions(UNICODE _UNICODE)
	# compile options utf-8 arch:avx2 fp:precise
	add_compile_options(/utf-8 /arch:AVX2 /fp:precise)
else()
	message(FATAL_ERROR "no proper compiler exist.")
endif()

# set c++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# RendererCore lib
file(GLOB_RECURSE LIBSOURCES ${CMAKE_SOURCE_DIR}/RendererCore/src/*.cpp
	${CMAKE_SOURCE_DIR}/RendererCore/src/gl.c)
add_library(RendererCore STATIC ${LIBSOURCES})
# RendererCore include path
target_include_directories(RendererCore PUBLIC ${CMAKE_SOURCE_DIR}/RendererCore/include)
# link DirectX libs
target_link_libraries(RendererCore PRIVATE dxgi d3d11)
if (CMAKE_BUILD_TYPE MATCHES Debug)
	target_link_libraries(RendererCore PRIVATE dxguid)
endif()

# TGRenderer exe
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/TGRenderer/src/*.cpp
	${CMAKE_SOURCE_DIR}/TGRenderer.rc)
# set entry point
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:wWinMainCRTStartup")
add_executable(TGRenderer ${SOURCES})
# TGRenderer include path
target_include_directories(TGRenderer PUBLIC ${CMAKE_SOURCE_DIR}/TGRenderer/include)
# link RendererCore lib
target_link_libraries(TGRenderer PRIVATE RendererCore)