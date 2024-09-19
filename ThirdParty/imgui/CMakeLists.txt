file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(imgui ${HEADERS} ${SOURCES})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/backends)

set_target_properties(imgui PROPERTIES FOLDER ${ThirdPartyFolderName})
