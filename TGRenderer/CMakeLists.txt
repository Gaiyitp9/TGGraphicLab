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

if (CMAKE_GENERATOR STREQUAL "Visual Studio 18 2026")
    set(EXECUTABLE_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/..)
elseif (CMAKE_GENERATOR STREQUAL "Ninja")
    set(EXECUTABLE_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
else()
    message(STATUS "Unknown generator: ${CMAKE_GENERATOR}")
endif()

add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/BuildTools/Version.py ${EXECUTABLE_OUTPUT_DIRECTORY}
    COMMENT "Generating GitLog.txt"
    VERBATIM
)

set(mimallocLibs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc.dll
        ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/mimalloc-redirect.dll)
add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${mimallocLibs} ${EXECUTABLE_OUTPUT_DIRECTORY}
    VERBATIM
)

set(ASSET_DIR ${CMAKE_SOURCE_DIR}/Assets)
file(GLOB_RECURSE ASSET_FILES ${ASSET_DIR}/*)
set(OUTPUT_ASSET_DIR ${EXECUTABLE_OUTPUT_DIRECTORY}/Assets)
foreach(ASSET_FILE ${ASSET_FILES})
    file(RELATIVE_PATH ASSET_RELATIVE_PATH ${ASSET_DIR} ${ASSET_FILE})
    set(ASSET_DEST_FILE ${OUTPUT_ASSET_DIR}/${ASSET_RELATIVE_PATH})
    get_filename_component(ASSET_DEST_DIR ${ASSET_DEST_FILE} DIRECTORY)

    add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSET_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSET_FILE} ${ASSET_DEST_FILE}
        VERBATIM
    )
endforeach ()
