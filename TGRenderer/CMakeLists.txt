file(GLOB_RECURSE SRCS *.cpp *.c TGRenderer.rc)
file(GLOB_RECURSE INCS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCS})

# set entry point
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:wWinMainCRTStartup")

add_executable(TGRenderer ${SRCS} ${INCS})
target_include_directories(TGRenderer PRIVATE include)
target_link_libraries(TGRenderer PRIVATE RenderCore)
