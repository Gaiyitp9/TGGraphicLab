add_compile_options(/utf-8)

add_executable(WinSocketTCPClient TCPClient.cpp)
add_executable(WinSocketTCPServer TCPServer.cpp)
add_executable(WinSocketUDPClient UDPClient.cpp)
add_executable(WinSocketUDPServer UDPServer.cpp)

target_link_libraries(WinSocketTCPClient PRIVATE "ws2_32.lib")
target_link_libraries(WinSocketTCPServer PRIVATE "ws2_32.lib")
target_link_libraries(WinSocketUDPClient PRIVATE "ws2_32.lib")
target_link_libraries(WinSocketUDPServer PRIVATE "ws2_32.lib")

# vs项目放在Tests目录下
set_target_properties(WinSocketTCPClient PROPERTIES FOLDER ${TestsFolderName})
set_target_properties(WinSocketTCPServer PROPERTIES FOLDER ${TestsFolderName})
set_target_properties(WinSocketUDPClient PROPERTIES FOLDER ${TestsFolderName})
set_target_properties(WinSocketUDPServer PROPERTIES FOLDER ${TestsFolderName})
