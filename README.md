# 天工图形代码库

[![GitHub](https://img.shields.io/badge/license-MIT-brightgreen)](https://opensource.org/licenses/MIT)

## 简介

“图形学第一定律”:

<p align="center"><i>if it looks right, it is right</i></p>
  刚看到这条“定律”时，我立马想起来程序开发时的一句话：“能跑就行”。但是这个幽默的说法包含了图形学一个重要思想——近似（嗯，不止是图形学）。毕竟，真实的光照效果极其复杂，而计算机的算力却有限。在学习图形学时，有些近似总会让人觉得：这也行吗？为了打消疑惑，加深理解，我们可以写代码跑起来并看是否能达到预期的渲染效果。“天工”取自“巧夺天工”，也是明代科技典籍《天工开物》之名，这道破了工艺的至高境界——人工的精巧胜过天然。在图形学的世界里，我们正是通过代码与算法（人工），去逼近和再现大自然的光影规律（天工），“天工图形代码库”正是在这样的想法下创建的。计划在里面加入各种图形学算法的实现，用来帮助更深入地理解这些算法，同时也为开发游戏引擎、DCC软件等工业级图形应用提供参考。

## 已实现的算法

- 数学库(包含图形学常用的计算)
- Blinn Phong光照模型
- 天空盒
- 部分几何体相交检测
- 基础版本路径追踪光线追踪

## 构建步骤

### 克隆项目

```bash
git clone --recurse-submodules https://github.com/Gaiyitp9/TGGraphicLab
```

如果已经拉取仓库，但是没有拉取子模块，那么在仓库的目录中运行

```bash
git submodule update --init --recursive
```

### 项目依赖

- Python
- Vulkan
- 如果需要构建和调试TG Header Tool，需要安装Clang。参考[官方教程](https://clang.llvm.org/docs/LibASTMatchersTutorial.html)编译LLVM项目，再运行 `cmake --install <build dir> --config <buildtype> --prefix <TGGraphicLab dir>/install/<buildtype>`安装到对应目录下

### 编译&调试

- 使用visual studio
  - 运行BuildTools/BuildAll.py，完成后会在build/TGGraphicLab目录下生成各种配置的文件夹。要使用VS调试时，打开配置文件夹下的sln文件即可。注意，在VS里运行时，Run Configuration要改为文件夹对应的配置，比如打开Debug文件夹下的sln时，Run Configuration就改为Debug
- 使用clion
  - 运行BuildTools/BuildDependencies.py，再用clion打开项目即可
- 运行TGRenderer时需要设置的环境变量： `MIMALLOC_DISABLE_REDIRECT=1` `MIMALLOC_SHOW_ERRORS=1` `MIMALLOC_SHOW_STATS=1`
  `MIMALLOC_VERBOSE=1`

## 参考资料

- [LearnOpenGL](https://learnopengl.com/)
- [PlanetChili](https://github.com/planetchili/hw3d)
- [Ray tracing in one weekend](https://raytracing.github.io/)
