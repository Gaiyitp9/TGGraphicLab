add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_definitions(spdlog INTERFACE SPDLOG_USE_STD_FORMAT)
