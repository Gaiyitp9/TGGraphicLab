file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(imgui ${HEADERS} ${SOURCES})
target_include_directories(imgui SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/backends)
target_link_libraries(imgui PUBLIC PowerVR)

set_target_properties(imgui PROPERTIES FOLDER ${ThirdPartyFolderName})
