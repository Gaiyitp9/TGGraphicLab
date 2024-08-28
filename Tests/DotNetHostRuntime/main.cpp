#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <windows.h>
#include <iostream>

namespace TG
{
    using String = std::basic_string<char_t>;
    hostfxr_initialize_for_dotnet_command_line_fn InitForCmdLineFPtr;
    hostfxr_initialize_for_runtime_config_fn InitForConfigFPtr;
    hostfxr_get_runtime_delegate_fn GetDelegateFPtr;
    hostfxr_run_app_fn RunAppFPtr;
    hostfxr_close_fn CloseAppFPtr;

    bool LoadHostfxr(char_t const* app);
    load_assembly_and_get_function_pointer_fn GetDotNetLoadAssembly(char_t const* assembly);

    int RunComponentExample(const String& rootPath);
    int RunAppExample(const String& rootPath);
}

int main(int argc, char* argv[])
{

    return 0;
}
