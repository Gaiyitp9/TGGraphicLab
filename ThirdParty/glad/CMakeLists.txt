file(GLOB_RECURSE HEADERS *.h *.hpp)
file(GLOB_RECURSE SOURCES *.cpp *.c)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

add_library(glad ${HEADERS} ${SOURCES})
target_include_directories(glad PUBLIC include)

configure_file(${CMAKE_CURRENT_LIST_DIR}/lib/libEGL.dll ${CMAKE_BINARY_DIR}/TGRenderer/libEGL.dll COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/lib/libGLESv2.dll ${CMAKE_BINARY_DIR}/TGRenderer/libGLESv2.dll COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/lib/vulkan-1.dll ${CMAKE_BINARY_DIR}/TGRenderer/vulkan-1.dll COPYONLY)
