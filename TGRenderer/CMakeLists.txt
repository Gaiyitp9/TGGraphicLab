file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE INCS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCS})

add_executable(TGRenderer ${SRCS} ${INCS})
target_include_directories(TGRenderer PRIVATE include)
target_link_libraries(TGRenderer PRIVATE RenderCore)
# set entry point
target_link_options(TGRenderer PRIVATE "/ENTRY:wWinMainCRTStartup")
