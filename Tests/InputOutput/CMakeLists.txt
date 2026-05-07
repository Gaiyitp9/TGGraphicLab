find_package(PkgConfig REQUIRED)
pkg_check_modules(EGL REQUIRED IMPORTED_TARGET egl)
pkg_check_modules(Wayland REQUIRED IMPORTED_TARGET wayland-client wayland-egl)

include(FetchContent)

FetchContent_Declare(
    ImGui
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/imgui"
)
FetchContent_MakeAvailable(ImGui)

add_executable(InputOutput main.cpp xdg-shell-client-protocol.c xdg-shell-client-protocol.h)

target_link_libraries(InputOutput PRIVATE PkgConfig::EGL PkgConfig::Wayland TG::ImGui)
# vs项目放在Tests目录下
set_target_properties(InputOutput PROPERTIES FOLDER ${TestsFolderName})
