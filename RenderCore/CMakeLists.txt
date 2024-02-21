file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE INCS *.hpp *.h *.inl)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCS})

add_library(RenderCore STATIC ${SRCS} ${INCS})
target_include_directories(RenderCore PUBLIC Include ThirdParty)
target_link_libraries(RenderCore PRIVATE dxgi d3d11 dxguid dbghelp opengl32)