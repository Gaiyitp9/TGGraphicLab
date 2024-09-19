find_package(Threads REQUIRED)

add_library(spdlog INTERFACE)
# 使用c++20的format库
target_compile_definitions(spdlog INTERFACE SPDLOG_USE_STD_FORMAT)
target_include_directories(spdlog SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(spdlog INTERFACE Threads::Threads)

set_target_properties(spdlog PROPERTIES FOLDER ${ThirdPartyFolderName})
