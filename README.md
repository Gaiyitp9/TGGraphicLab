# 天工图形代码库

[![GitHub](https://img.shields.io/badge/license-MIT-brightgreen)](https://opensource.org/licenses/MIT)

## 简介

“图形学第一定律”：

<p align="center"><i>if it looks right, it is right</i></p>
  第一次看到这条“定律”时，我立马想起来程序开发时的一句话：“能跑就行:)”。可能提出这个说法的人确实有这种想法，但是这个幽默的说法却包含了图形学一个重要思想——近似（嗯，不止是图形学）。毕竟，真实的光照效果极其复杂，而计算机的算力却有限。在学习图形学时，有些近似总会让人觉得：这也行吗？与其在纸上纠结，不如写成代码跑起来，看是否能达到预期的渲染效果。“天工”取自“巧夺天工”，也是明代科技典籍《天工开物》之名，这道破了工艺的至高境界——人工的精巧胜过天然。在图形学的世界里，我们正是通过代码与算法（人工），去逼近和再现大自然的光影规律（天工），“天工图形代码库”正是在这样的想法下创建的。计划在里面加入各种图形学算法的实现，用来帮助更深入地理解这些算法，同时也为开发游戏引擎、DCC软件等工业级图形应用提供参考。

## 已实现的算法

- 数学库(包含图形学常用的计算)
- Blinn Phong、PBR光照模型
- 天空盒
- 部分几何体相交检测
- 基础版本路径追踪光线追踪

## 构建步骤

### 克隆项目

```bash
git clone https://github.com/Gaiyitp9/TGGraphicLab
```



### 项目依赖

- Windows
  - Python
  - Vulkan
- Ubuntu
  - PkgConfig
    ```bash
    sudo apt install pkg-config
    ```
  - OpenGL
    ```bash
    sudo apt install libgl1-mesa-dev libegl1-mesa-dev
    ``` 
  - Vulkan  
    &emsp;&emsp;下载vulkansdk-linux-x86_64-1.4.x.x.tar.xz并解压。在~/.bashrc中添加`source vulkan/path/1.4.x.x/setup-env.sh`，用于设置终端的Vulkan环境变量；在~/.profile中添加`. vulkan/path/1.4.x.x/setup-env.sh`，用于设置GUI程序的Vulkan环境变量，比如使用CLion调试项目时，CLion需要读取Vulkan环境变量让CMake能通过find_package找到Vulkan路径
  - Wayland
    ```bash
    sudo apt install libwayland-dev wayland-protocols libxkbcommon-dev
    ```
- 如果需要构建和调试TG Header Tool，需要安装Clang。参考[官方教程](https://clang.llvm.org/docs/LibASTMatchersTutorial.html)编译LLVM项目，再运行 `cmake --install <build dir> --config <build type> --prefix <TGGraphicLab dir>/install/<build type>`安装到对应目录下

### 编译&调试

- CLion
  - 直接使用CLion打开项目目录
- Visual Studio 2026
  - 在项目目录下运行
    ```powershell
    cmake -S . --preset visual-studio
    ```
    完成后用Visual Studio打开在build目录下的TGGraphicLab.sln
- 如果需要查看mimalloc的状态，运行TGRenderer时设置环境变量：`MIMALLOC_VERBOSE=1` `MIMALLOC_SHOW_ERRORS=1` `MIMALLOC_SHOW_STATS=1`。如果不需要重定向malloc等函数，可以设置环境变量：`MIMALLOC_DISABLE_REDIRECT=0`

## 参考资料

- [LearnOpenGL](https://learnopengl.com/)
- [PlanetChili](https://github.com/planetchili/hw3d)
- [Ray tracing in one weekend](https://raytracing.github.io/)
