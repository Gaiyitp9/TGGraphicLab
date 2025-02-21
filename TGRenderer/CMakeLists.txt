set(EXECUTABLE_NAME TGRenderer)

file(GLOB_RECURSE SOURCES *.cpp *.c)
file(GLOB_RECURSE HEADERS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${HEADERS} ${SOURCES})

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Private)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE GraphicCore)

include(GNUInstallDirs)

set(EGLLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/LibEGL.dll)
set(GLESLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/LibGLESv2.dll)
add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${EGLLib} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy ${GLESLib} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
    COMMENT "Copying EGL and GLES dynamic libraries to executable output directory"
)

set(mimallocLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc$<$<CONFIG:DEBUG>:-debug>.dll)
set(mimallocRedirectLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc-redirect.dll)
add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${mimallocLib} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${mimallocRedirectLib} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        COMMENT "Copying mimalloc dynamic libraries to executable output directory"
)

set(ICONS ${CMAKE_CURRENT_LIST_DIR}/maple-leaf.ico ${CMAKE_CURRENT_LIST_DIR}/maple-leaf2.ico)
add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${ICONS} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        COMMENT "Copying icons to executable output directory"
)

set(SHADER_DIR ${CMAKE_SOURCE_DIR}/Shaders)
add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SHADER_DIR} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/Shaders
        COMMENT "Copying shaders to executable output directory"
)
