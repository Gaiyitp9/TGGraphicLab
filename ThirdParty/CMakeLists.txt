cmake_minimum_required(VERSION 3.29)

project(TGThirdParty)

# 添加所有三方库
add_subdirectory(googletest)
add_subdirectory(PowerVR)
add_subdirectory(imgui)
add_subdirectory(mimalloc)
add_subdirectory(spdlog)
