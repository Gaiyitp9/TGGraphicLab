file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE INCS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCS})

add_executable(TGRenderer ${SRCS} ${INCS})
target_include_directories(TGRenderer PRIVATE Include)
target_link_libraries(TGRenderer PRIVATE GraphicCore)

set(PowerVRResources ${CMAKE_SOURCE_DIR}/ThirdParty/PowerVR/lib/Windows)

add_custom_command(
    TARGET TGRenderer
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PowerVRResources} $<TARGET_FILE_DIR:TGRenderer>
    COMMENT "Copying EGL and GLES dynamic libraries to executable output directory"
)

set(ICONS ${CMAKE_CURRENT_SOURCE_DIR}/maple-leaf.ico ${CMAKE_CURRENT_SOURCE_DIR}/maple-leaf2.ico)
add_custom_command(
        TARGET TGRenderer
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${ICONS} $<TARGET_FILE_DIR:TGRenderer>
        COMMENT "Copying icons to executable output directory"
)
