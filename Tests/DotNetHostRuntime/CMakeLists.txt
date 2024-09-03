add_executable(DotNetHostRuntime main.cpp)

set(DotNetPath "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.8/runtimes/win-x64/native")

target_include_directories(DotNetHostRuntime PRIVATE ${DotNetPath})
target_link_directories(DotNetHostRuntime PRIVATE ${DotNetPath})
target_link_libraries(DotNetHostRuntime PRIVATE nethost)

# vs项目放在Tests目录下
set_target_properties(DotNetHostRuntime PROPERTIES FOLDER ${TestsFolderName})

add_custom_command(TARGET DotNetHostRuntime POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DotNetPath}/nethost.dll
        ${CMAKE_BINARY_DIR}/Tests/DotNetHostRuntime
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/DotNetLib/bin/$<CONFIG>/net8.0/DotNetLib.dll
        ${CMAKE_BINARY_DIR}/Tests/DotNetHostRuntime
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/PInvokeExample/bin/$<CONFIG>/net8.0/PInvokeExample.dll
        ${CMAKE_BINARY_DIR}/Tests/DotNetHostRuntime
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/PInvokeExample/bin/$<CONFIG>/net8.0/PInvokeExample.runtimeconfig.json
        ${CMAKE_BINARY_DIR}/Tests/DotNetHostRuntime
)
