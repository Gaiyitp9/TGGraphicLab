add_compile_options(/utf-8)

add_executable(WinSocketTCPClient TCPClient.cpp)
add_executable(WinSocketTCPServer TCPServer.cpp)
add_executable(WinSocketUDPClient UDPClient.cpp)
add_executable(WinSocketUDPServer UDPServer.cpp)

target_link_libraries(WinSocketTCPClient PRIVATE "ws2_32.lib")
target_link_libraries(WinSocketTCPServer PRIVATE "ws2_32.lib")
target_link_libraries(WinSocketUDPClient PRIVATE "ws2_32.lib")
target_link_libraries(WinSocketUDPServer PRIVATE "ws2_32.lib")
