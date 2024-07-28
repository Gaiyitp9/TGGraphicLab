#include <print>
#include <iostream>
#include <thread>
#include <chrono>
#include <corecrt_io.h>
#include <windows.h>

bool IsConsoleStream(FILE* stream)
{
    // 获取文件流的文件描述符
    int fileDescriptor = _fileno(stream);

    // 获取文件描述符对应的操作系统文件句柄
    intptr_t osFileHandle = _get_osfhandle(fileDescriptor);

    // 检查文件句柄是否有效
    if (osFileHandle == -1)
        return false;

    // 检查文件句柄是否指向一个控制台
    DWORD mode;
    return GetConsoleMode(reinterpret_cast<HANDLE>(osFileHandle), &mode) != 0;
}

int main()
{
    // For some systems, this mode provides line buffering. However,
    // for Win32, the behavior is the same as _IOFBF - Full Buffering.
    // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setvbuf?view=msvc-170
    // setvbuf(stdout, NULL, _IOFBF, 1024);
    setvbuf(stdout, nullptr, _IONBF, 0);

    if (IsConsoleStream(stdout)) {
        std::cout << "stdout is connected to a console.\n";
    } else {
        std::cout << "stdout is not connected to a console.\n";
    }

    std::cout << "Hello, without flush";
    std::print("Hello, without flush");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
