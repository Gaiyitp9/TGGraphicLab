file(GLOB_RECURSE SRCS *.cpp *.c)
file(GLOB_RECURSE INCS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${INCS})

find_package(mimalloc 2.1.7 REQUIRED)

set(PROJECT_NAME TGRenderer)
add_executable(${PROJECT_NAME} ${SRCS} ${INCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Private)
target_link_libraries(${PROJECT_NAME} PRIVATE GraphicCore mimalloc)

include(GNUInstallDirs)
set(EGLLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/LibEGL.dll)
set(GLESLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/LibGLESv2.dll)
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${EGLLib} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy ${GLESLib} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying EGL and GLES dynamic libraries to executable output directory"
)

include(GNUInstallDirs)

set(mimallocLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc$<$<CONFIG:DEBUG>:-debug>.dll)
set(mimallocRedirectLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc-redirect.dll)
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${mimallocLib} $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${mimallocRedirectLib} $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying mimalloc dynamic libraries to executable output directory"
)

set(ICONS ${CMAKE_CURRENT_LIST_DIR}/maple-leaf.ico ${CMAKE_CURRENT_LIST_DIR}/maple-leaf2.ico)
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${ICONS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying icons to executable output directory"
)
