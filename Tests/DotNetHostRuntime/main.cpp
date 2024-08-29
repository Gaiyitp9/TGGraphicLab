#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <windows.h>
#include <print>
#include <cassert>
#include <vector>
#include <thread>
#include <chrono>

namespace TG
{
    hostfxr_initialize_for_dotnet_command_line_fn InitForCmdLineFPtr;
    hostfxr_initialize_for_runtime_config_fn InitForConfigFPtr;
    hostfxr_get_runtime_delegate_fn GetDelegateFPtr;
    hostfxr_run_app_fn RunAppFPtr;
    hostfxr_close_fn CloseFPtr;

    bool LoadHostfxr(wchar_t const* assembly_path);
    load_assembly_and_get_function_pointer_fn GetDotNetLoadAssembly(wchar_t const* configPath);

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
        const std::wstring configPath = rootPath + L"PInvokeExample.runtimeconfig.json";
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

        // Function pointer to managed delegate with non-default signature
        using CustomEntryPointFn = void(CORECLR_DELEGATE_CALLTYPE*)(LibArgs);
        CustomEntryPointFn custom;
        LibArgs args{L"from host!", -1};

        // UnmanagedCallersOnly
        rc = loadAssemblyAndGetFunctionPointer(
            dotnetlibPath.c_str(),
            dotnetType,
            L"CustomEntryPointUnmanagedCallersOnly",
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            reinterpret_cast<void**>(&custom));
        assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        custom(args);

        // Custom delegate type
        rc = loadAssemblyAndGetFunctionPointer(
            dotnetlibPath.c_str(),
            dotnetType,
            L"CustomEntryPoint",
            L"DotNetLib.Lib+CustomEntryPointDelegate, DotNetLib",
            nullptr,
            reinterpret_cast<void**>(&custom));
        assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        custom(args);

        return EXIT_SUCCESS;
    }

    int RunAppExample(const std::wstring &rootPath)
    {
        const std::wstring appPath = rootPath + L"PInvokeExample.dll";

        if (!LoadHostfxr(appPath.c_str()))
        {
            assert(false && "Failure: LoadHostfxr()");
            return EXIT_FAILURE;
        }

        // Load .NET Core
        hostfxr_handle cxt = nullptr;
        std::vector args { appPath.c_str(), L"app_arg_1", L"app_arg_2" };
        int rc = InitForCmdLineFPtr(args.size(), args.data(), nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            std::print(stderr, "Init failed: {0:#x}\n", rc);
            CloseFPtr(cxt);
            return EXIT_FAILURE;
        }

        // Get the function pointer to get function pointers
        get_function_pointer_fn getFunctionPointer = nullptr;
        rc = GetDelegateFPtr(cxt, hdt_get_function_pointer, reinterpret_cast<void**>(&getFunctionPointer));
        if (rc != 0 || getFunctionPointer == nullptr)
            std::print(stderr, "GetDelegateFPtr failed: {0:#x}\n", rc);

        // Function pointer to App.IsWaiting
        using IsWaitingFn = unsigned char (CORECLR_DELEGATE_CALLTYPE*)();
        IsWaitingFn isWaitingFn = nullptr;
        rc = getFunctionPointer(L"PInvokeExample.App, PInvokeExample", L"IsWaiting",
            UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&isWaitingFn));
        assert(rc == 0 && isWaitingFn != nullptr && "Failure: GetDelegateFPtr()");

        // Function pointer to App.Hello
        using HelloFn = unsigned char (CORECLR_DELEGATE_CALLTYPE*)(char const*);
        HelloFn hello = nullptr;
        rc = getFunctionPointer(L"PInvokeExample.App, PInvokeExample", L"Hello",
            UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&hello));
        assert(rc == 0 && hello != nullptr && "Failure: GetDelegateFPtr()");

        // Invoke the function in a different from the main app
        std::thread t([&] {
           while (isWaitingFn() != 1)
               std::this_thread::sleep_for(std::chrono::milliseconds(100));

            for (int i = 0; i < 3; ++i)
                hello("from host!");
        });

        RunAppFPtr(cxt);
        t.join();

        CloseFPtr(cxt);
        return EXIT_SUCCESS;
    }

    // Using the nethost library, discover the location of hostfxr and get exports
    bool LoadHostfxr(wchar_t const* assembly_path)
    {
        get_hostfxr_parameters params { sizeof(get_hostfxr_parameters), assembly_path, nullptr };
        // Pre-allocate a large buffer for the path to hostfxr
        wchar_t buffer[MAX_PATH];
        std::size_t buffer_size = sizeof(buffer) / sizeof(wchar_t);
        int rc = get_hostfxr_path(buffer, &buffer_size, &params);
        if (rc != 0)
            return false;

        // Load hostfxr and get desired exports
        // NOTE: The .NET Runtime does not support unloading any of its native libraries. Running
        // dlclose/FreeLibrary on any .NET libraries produces undefined behavior.
        HMODULE lib = LoadLibraryW(buffer);
        assert(lib != nullptr);
        InitForCmdLineFPtr = reinterpret_cast<hostfxr_initialize_for_dotnet_command_line_fn>(GetProcAddress(
            lib, "hostfxr_initialize_for_dotnet_command_line"));
        InitForConfigFPtr = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(GetProcAddress(
            lib, "hostfxr_initialize_for_runtime_config"));
        GetDelegateFPtr = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(GetProcAddress(
            lib, "hostfxr_get_runtime_delegate"));
        RunAppFPtr = reinterpret_cast<hostfxr_run_app_fn>(GetProcAddress(lib, "hostfxr_run_app"));
        CloseFPtr = reinterpret_cast<hostfxr_close_fn>(GetProcAddress(lib, "hostfxr_close"));

        return InitForCmdLineFPtr && InitForConfigFPtr && GetDelegateFPtr && RunAppFPtr && CloseFPtr;
    }

    load_assembly_and_get_function_pointer_fn GetDotNetLoadAssembly(wchar_t const* configPath)
    {
        void* loadAssemblyAndGetFunctionPointer = nullptr;
        hostfxr_handle cxt = nullptr;
        int rc = InitForConfigFPtr(configPath, nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            std::print(stderr, "Init failed: {0:#x}\n", rc);
            CloseFPtr(cxt);
            return nullptr;
        }

        rc = GetDelegateFPtr(cxt, hdt_load_assembly_and_get_function_pointer, &loadAssemblyAndGetFunctionPointer);
        if (rc != 0 || loadAssemblyAndGetFunctionPointer == nullptr)
            std::print(stderr, "GetDelegateFPtr failed: {0:#x}\n", rc);

        CloseFPtr(cxt);
        return reinterpret_cast<load_assembly_and_get_function_pointer_fn>(loadAssemblyAndGetFunctionPointer);
    }
}
