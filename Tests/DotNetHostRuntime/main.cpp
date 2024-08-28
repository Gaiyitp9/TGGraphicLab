#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <windows.h>
#include <iostream>
#include <cassert>

namespace TG
{
    hostfxr_initialize_for_dotnet_command_line_fn InitForCmdLineFPtr;
    hostfxr_initialize_for_runtime_config_fn InitForConfigFPtr;
    hostfxr_get_runtime_delegate_fn GetDelegateFPtr;
    hostfxr_run_app_fn RunAppFPtr;
    hostfxr_close_fn CloseAppFPtr;

    bool LoadHostfxr(wchar_t const* app);
    load_assembly_and_get_function_pointer_fn GetDotNetLoadAssembly(wchar_t const* assembly);

    int RunComponentExample(const std::wstring& rootPath);
    int RunAppExample(const std::wstring& rootPath);
}

int wmain(int argc, wchar_t* argv[])
{
    wchar_t hostPath[MAX_PATH];
    DWORD size = GetFullPathNameW(argv[0], sizeof(hostPath) / sizeof(wchar_t), hostPath, nullptr);
    assert(size != 0);

    std::wstring rootPath(hostPath);
    auto pos = rootPath.find_last_of(L'\\');
    assert(pos != std::wstring::npos);
    rootPath = rootPath.substr(0, pos + 1);

    if (argc > 1 && wcscmp(argv[1], L"app") == 0)
        return TG::RunAppExample(rootPath);
    else
        return TG::RunComponentExample(rootPath);
}

namespace TG
{
    struct LibArgs
    {
        wchar_t const* message;
        int number;
    };

    int RunComponentExample(const std::wstring& rootPath)
    {
        // step 1: Load HostFxr and get exported hosting functions
        if (!LoadHostfxr(nullptr))
        {
            assert(false && "Failure: LoadHostfxr()");
            return EXIT_FAILURE;
        }

        // step 2: Initialize and start the .NET core runtime
        const std::wstring configPath = rootPath + L"DotNetLib.runtimeconfig.json";
        load_assembly_and_get_function_pointer_fn loadAssemblyAndGetFunctionPointer = nullptr;
        loadAssemblyAndGetFunctionPointer = GetDotNetLoadAssembly(configPath.c_str());
        assert(loadAssemblyAndGetFunctionPointer != nullptr && "Failure: GetDotNetLoadAssembly()");

        // step 3: Load managed assembly and get function pointer to a managed method
        const std::wstring dotnetlibPath = rootPath + L"DotNetLib.dll";
        wchar_t const* dotnetType = L"DotNetLib.Lib, DotNetLib";
        wchar_t const* dotnetTypeMethod = L"Hello";

        // Function pointer to managed delegate
        component_entry_point_fn hello = nullptr;
        int rc = loadAssemblyAndGetFunctionPointer(
            dotnetlibPath.c_str(),
            dotnetType,
            dotnetTypeMethod,
            nullptr,
            nullptr,
            reinterpret_cast<void**>(&hello));
        assert(rc == 0 && hello != nullptr && "Failure: load_assembly_and_get_function_pointer()");

        // step 4: Run managed code
        for (int i = 0; i < 3; ++i)
        {
            LibArgs args{L"from host!", i};
            hello(&args, sizeof(args));
        }
    }

    void* CustomLoadLibrary(const char_t *path)
    {
        HMODULE h = ::LoadLibraryW(path);
        assert(h != nullptr);
        return (void*)h;
    }
    void* GetExport(void *h, const char *name)
    {
        void* f = ::GetProcAddress(static_cast<HMODULE>(h), name);
        assert(f != nullptr);
        return f;
    }

    // Using the nethost library, discover the location of hostfxr and get exports
    bool LoadHostfxr(const char_t *assembly_path)
    {
        get_hostfxr_parameters params { sizeof(get_hostfxr_parameters), assembly_path, nullptr };
        // Pre-allocate a large buffer for the path to hostfxr
        char_t buffer[MAX_PATH];
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);
        int rc = get_hostfxr_path(buffer, &buffer_size, &params);
        if (rc != 0)
            return false;

        // Load hostfxr and get desired exports
        // NOTE: The .NET Runtime does not support unloading any of its native libraries. Running
        // dlclose/FreeLibrary on any .NET libraries produces undefined behavior.
        void *lib = CustomLoadLibrary(buffer);
        InitForCmdLineFPtr = (hostfxr_initialize_for_dotnet_command_line_fn)GetExport(lib, "hostfxr_initialize_for_dotnet_command_line");
        InitForConfigFPtr = (hostfxr_initialize_for_runtime_config_fn)GetExport(lib, "hostfxr_initialize_for_runtime_config");
        GetDelegateFPtr = (hostfxr_get_runtime_delegate_fn)GetExport(lib, "hostfxr_get_runtime_delegate");
        RunAppFPtr = (hostfxr_run_app_fn)GetExport(lib, "hostfxr_run_app");
        CloseAppFPtr = (hostfxr_close_fn)GetExport(lib, "hostfxr_close");

        return InitForConfigFPtr && GetDelegateFPtr && CloseAppFPtr;
    }
}
