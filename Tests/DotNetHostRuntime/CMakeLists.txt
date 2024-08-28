add_executable(DotNetHostRuntime main.cpp)

target_include_directories(DotNetHostRuntime PRIVATE
        "C:\\Program Files\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x64\\8.0.8\\runtimes\\win-x64\\native")
# vs项目放在Tests目录下
set_target_properties(DotNetHostRuntime PROPERTIES FOLDER ${TestsFolderName})
