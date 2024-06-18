file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.c)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

add_library(glad STATIC ${HEADERS} ${SOURCES})
target_include_directories(glad PUBLIC include)
set_target_properties(glad PROPERTIES FOLDER ${ThirdPartyFolderName})

# google ANGLE编译的动态库
configure_file(${CMAKE_CURRENT_LIST_DIR}/lib/libEGL.dll ${CMAKE_BINARY_DIR}/TGRenderer/libEGL.dll COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/lib/libGLESv2.dll ${CMAKE_BINARY_DIR}/TGRenderer/libGLESv2.dll COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/lib/vulkan-1.dll ${CMAKE_BINARY_DIR}/TGRenderer/vulkan-1.dll COPYONLY)
