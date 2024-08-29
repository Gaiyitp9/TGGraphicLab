add_executable(DotNetHostRuntime main.cpp)

set(DotNetPath "C:\\Program Files\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x64\\8.0.8\\runtimes\\win-x64\\native")

target_include_directories(DotNetHostRuntime PRIVATE ${DotNetPath})
target_link_directories(DotNetHostRuntime PRIVATE ${DotNetPath})
target_link_libraries(DotNetHostRuntime PRIVATE nethost)
set(CMAKE_BUILD_RPATH ${DotNetPath})
# vs项目放在Tests目录下
set_target_properties(DotNetHostRuntime PROPERTIES FOLDER ${TestsFolderName})
