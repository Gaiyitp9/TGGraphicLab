add_compile_options(/utf-8)

add_executable(WinSocketClient client.cpp)
add_executable(WinSocketServer server.cpp)

target_link_libraries(WinSocketClient PRIVATE "ws2_32.lib")
target_link_libraries(WinSocketServer PRIVATE "ws2_32.lib")
