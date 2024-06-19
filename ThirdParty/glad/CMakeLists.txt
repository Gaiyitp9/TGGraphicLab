file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.c)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

add_library(glad STATIC ${HEADERS} ${SOURCES})
target_include_directories(glad PUBLIC include)
set_target_properties(glad PROPERTIES FOLDER ${ThirdPartyFolderName})
