set(PROJECT_NAME TGRenderer)

file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE INCS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCS})

add_executable(${PROJECT_NAME} ${SRCS} ${INCS})
target_include_directories(${PROJECT_NAME} PRIVATE Include)
target_link_libraries(${PROJECT_NAME} PRIVATE GraphicCore mimalloc)

set(PowerVRResources ${CMAKE_SOURCE_DIR}/ThirdParty/PowerVR/lib/Windows)
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PowerVRResources} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying EGL and GLES dynamic libraries to executable output directory"
)

set(mimallocLibraries ${CMAKE_SOURCE_DIR}/ThirdParty/mimalloc/lib/Windows/$<CONFIG>)
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${mimallocLibraries} $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying mimalloc dynamic libraries to executable output directory"
)

set(ICONS ${CMAKE_CURRENT_SOURCE_DIR}/maple-leaf.ico ${CMAKE_CURRENT_SOURCE_DIR}/maple-leaf2.ico)
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${ICONS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying icons to executable output directory"
)
