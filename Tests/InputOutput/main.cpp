// #include <print>
// #include <thread>
// #include <chrono>
// #include <corecrt_io.h>
// #include <windows.h>
//
// bool IsConsoleStream(FILE* stream)
// {
//     // 获取文件流的文件描述符
//     int fileDescriptor = _fileno(stream);
//
//     // 获取文件描述符对应的操作系统文件句柄
//     intptr_t osFileHandle = _get_osfhandle(fileDescriptor);
//
//     // 检查文件句柄是否有效
//     if (osFileHandle == -1)
//         return false;
//
//     // 检查文件句柄是否指向一个控制台
//     DWORD mode;
//     return GetConsoleMode(reinterpret_cast<HANDLE>(osFileHandle), &mode) != 0;
// }
//
// int main()
// {
//     // For some systems, this mode provides line buffering. However,
//     // for Win32, the behavior is the same as _IOFBF - Full Buffering.
//     // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setvbuf?view=msvc-170
//     // setvbuf(stdout, NULL, _IOLBF, 1024);
//     setvbuf(stdout, NULL, _IOFBF, 1024);
//     // setvbuf(stdout, nullptr, _IONBF, 0);
//
//     if (IsConsoleStream(stdout))
//         std::println("stdout is connected to a console.");
//     else
//         std::println("stdout is not connected to a console.");
//
//     // 测试发现Clion的控制台可以设置buffer模式
//     // Windows的cmd和powershell无法设置，默认没有buffering，即_IONBF
//     std::println("Hello, with flush");
//     std::fflush(stdout);
//     std::print("Hello, without flush");
//     std::this_thread::sleep_for(std::chrono::seconds(2));
//     return 0;
// }

#include <print>
#include <cstring>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include "xdg-shell-client-protocol.h"
#include "imgui_impl_opengl3.h"

constexpr int width = 640;
constexpr int height = 480;

wl_compositor* compositor = nullptr;
xdg_wm_base* xdgWMBase = nullptr;

void XDGWMBasePing(void* data, xdg_wm_base* xdgWMBase_, uint32_t serial)
{
    xdg_wm_base_pong(xdgWMBase_, serial);
}

xdg_wm_base_listener xdgWMBaseListener{
    .ping = XDGWMBasePing
};

void RegistryGlobal(void* data, wl_registry* registry, uint32_t name,
    const char* interface, uint32_t version)
{
    if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        compositor = static_cast<wl_compositor*>(
            wl_registry_bind(registry, name, &wl_compositor_interface, version));
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        xdgWMBase = static_cast<xdg_wm_base*>(
            wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
        xdg_wm_base_add_listener(xdgWMBase, &xdgWMBaseListener, nullptr);
    }
}

void RegistryGlobalRemove(void *data, wl_registry *wl_registry, uint32_t name)
{

}

void XDGSurfaceConfigure(void* data, xdg_surface* xdgSurface,
              uint32_t serial)
{
    xdg_surface_ack_configure(xdgSurface, serial);
}

xdg_surface_listener xdgSurfaceListener{
    .configure = XDGSurfaceConfigure
};

bool running = true;
void ToplevelClose(void* data, xdg_toplevel* toplevel)
{
    running = false;
}

void ToplevelConfigure(void *data, xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, wl_array *states)
{

}

void ToplevelConfigureBounds(void* data, xdg_toplevel* xdg_toplevel, int32_t width, int32_t height)
{

}

void ToplevelWMCapabilities(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities)
{

}

const xdg_toplevel_listener toplevelListener = {
    .configure = ToplevelConfigure,
    .close = ToplevelClose,
    .configure_bounds = ToplevelConfigureBounds,
    .wm_capabilities = ToplevelWMCapabilities
};

void Draw()
{
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello");
    ImGui::Text("Wayland + OpenGL + ImGui");
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
    // 1. 连接到wayland compositor
    wl_display* display = wl_display_connect(nullptr);
    if (!display)
    {
        std::println("Can not connect to wayland compositor");
        return 1;
    }

    // 2. 获取registry并绑定全局接口
    // Registry 是 Wayland 的"服务发现中心"
    // Compositor 启动时会向 registry 注册它支持的所有全局对象
    // 客户端连接后，通过 registry 事件知道有哪些服务可用，然后用 wl_registry_bind 绑定到自己需要的版本。
    wl_registry* registry = wl_display_get_registry(display);
    const wl_registry_listener registryListener{
        .global = RegistryGlobal,
        .global_remove = RegistryGlobalRemove
    };
    wl_registry_add_listener(registry, &registryListener, nullptr);
    // Wayland 是异步的。add_listener 只是注册回调，不会立即收到事件
    // roundtrip 发送一个特殊请求并等待回复，确保在此期间的所有 registry 事件都被处理
    wl_display_roundtrip(display);
    if (!compositor || !xdgWMBase)
    {
        std::println("Lack of necessary wayland interface");
        return 1;
    }

    // 3. 创建surface
    wl_surface* surface = wl_compositor_create_surface(compositor);
    xdg_surface* xdgSurface = xdg_wm_base_get_xdg_surface(xdgWMBase, surface);
    xdg_surface_add_listener(xdgSurface, &xdgSurfaceListener, nullptr);
    xdg_toplevel* xdgTopLevel = xdg_surface_get_toplevel(xdgSurface);
    xdg_toplevel_add_listener(xdgTopLevel, &toplevelListener, nullptr);
    xdg_toplevel_set_title(xdgTopLevel, "TGGraphicLab");
    xdg_toplevel_set_app_id(xdgTopLevel, "com.example.wayland-test");

    wl_surface_commit(surface);
    wl_display_roundtrip(display);

    // 4. 初始化egl
    EGLDisplay eglDisplay = eglGetDisplay(display);
    if (eglDisplay == EGL_NO_DISPLAY)
    {
        EGLint error = eglGetError();
        if (error != EGL_SUCCESS)
            std::println("Failed to get EGL display 0x{:x}", error);
        return 1;
    }

    int major, minor;
    if (!eglInitialize(eglDisplay, &major, &minor))
    {
        std::println("Failed to initialize EGL");
        return 1;
    }
    std::println("EGL version: {}.{}", major, minor);

    // 3. 绑定API
    eglBindAPI(EGL_OPENGL_API);

    // 4. 选择配置
    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };

    EGLConfig config;
    EGLint num_configs;
    eglChooseConfig(eglDisplay, configAttribs, &config, 1, &num_configs);

    // 5. 创建OpenGL 4.6 Context
    EGLint ctxAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 4,
        EGL_CONTEXT_MINOR_VERSION, 6,
        EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
        EGL_NONE
    };
    EGLContext context = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, ctxAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        EGLint error = eglGetError();
        std::println("eglCreateContext failed with error: {:x}", error);
        return 1;
    }

    wl_egl_window* eglWindow = wl_egl_window_create(surface, width, height);
    // 6. 创建Surface
    EGLSurface eglSurface = eglCreateWindowSurface(
        eglDisplay,
        config,
        eglWindow,
        nullptr
    );
    if (eglSurface == EGL_NO_SURFACE)
    {
        EGLint error = eglGetError();
        std::println("eglCreateWindowSurface failed with error: 0x{:x}", error);
        return 1;
    }

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, context);

    // 加载OpenGL函数
    gladLoaderLoadGL();

    const GLubyte* version = glGetString(GL_VERSION);
    std::println("GL_VERSION: {}", reinterpret_cast<const char*>(version));

    // 13. 初始化ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    ImGui_ImplOpenGL3_Init();

    while (running)
    {
        wl_display_dispatch_pending(display);
        Draw();
        eglSwapBuffers(eglDisplay, eglSurface);
    }

    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(eglDisplay, eglSurface);
    eglDestroyContext(eglDisplay, context);
    eglTerminate(eglDisplay);

    xdg_toplevel_destroy(xdgTopLevel);
    xdg_surface_destroy(xdgSurface);
    wl_surface_destroy(surface);
    wl_display_disconnect(display);

    return 0;
}
