find_package(Python3 3.8 REQUIRED)
if (NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found!")
endif()

set(EXECUTABLE_NAME "TGRenderer")

file(GLOB_RECURSE SOURCES *.cpp *.c)
file(GLOB_RECURSE HEADERS *.h *.hpp)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${HEADERS} ${SOURCES})

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Private)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE GraphicCore)

include(GNUInstallDirs)

if (CMAKE_GENERATOR STREQUAL "Visual Studio 17 2022")
    set(EXECUTABLE_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/..)
elseif (CMAKE_GENERATOR STREQUAL "Ninja")
    set(EXECUTABLE_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
else()
    message(STATUS "Unknown generator: ${CMAKE_GENERATOR}")
endif()

set(EGLLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/LibEGL.dll)
set(GLESLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/LibGLESv2.dll)
set(mimallocLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc$<$<CONFIG:DEBUG>:-debug>.dll)
set(mimallocRedirectLib ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc-redirect.dll)
set(ICONS ${CMAKE_CURRENT_SOURCE_DIR}/maple-leaf.ico ${CMAKE_CURRENT_SOURCE_DIR}/maple-leaf2.ico)
add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EGLLib} ${EXECUTABLE_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLESLib} ${EXECUTABLE_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${mimallocLib} ${EXECUTABLE_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${mimallocRedirectLib} ${EXECUTABLE_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ICONS} ${EXECUTABLE_OUTPUT_DIRECTORY}
    COMMENT "Copying dynamic libraries and icon resources to executable output directory"
)

set(SHADER_DIR ${CMAKE_SOURCE_DIR}/Shaders)
add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_DIR} ${EXECUTABLE_OUTPUT_DIRECTORY}/Shaders
    COMMENT "Copying shaders to executable output directory"
)

add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/BuildTools/Version.py ${EXECUTABLE_OUTPUT_DIRECTORY}
    COMMENT "Generating GitLog.txt"
)
