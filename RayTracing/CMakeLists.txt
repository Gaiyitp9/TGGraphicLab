file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE INCS *.hpp *.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCS})

add_executable(RayTracing ${SRCS} ${INCS})
target_include_directories(RayTracing PRIVATE ../ThirdParty)