file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE INCLUDES *.hpp *.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCLUDES})

add_executable(RayTracing ${SOURCES} ${INCLUDES})
target_link_libraries(RayTracing PRIVATE stb)
