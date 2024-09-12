add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# 使用c++20的format库
target_compile_definitions(spdlog INTERFACE SPDLOG_USE_STD_FORMAT)

set_target_properties(PowerVR PROPERTIES FOLDER ${ThirdPartyFolderName})
