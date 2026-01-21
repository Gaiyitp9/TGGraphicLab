# 天工图形代码库
[![GitHub](https://img.shields.io/badge/license-MIT-brightgreen)](https://opensource.org/licenses/MIT)

## 简介
"图形学第一定律":
<p style="text-align: center;"><i>if it looks right, it is right</i></p>
这个"定律"表面上看上去太主观，违背了科学的严谨和客观性，怎么理解呢？事实上，这种幽默的说法是为了说明图形学追求的是让人在视觉上令人信服，而不是完美的数学和物理准确性，或者说，图形学算法采用了非常多的近似。另一方面，我们是不是可以这样来理解：图形学十分强调实践，当你在学习一个图形学算法时，如果你不能在代码里实现并呈现出正确的视觉效果，那么可以认为你没有完全理解。天工算法代码库就是在这种想法下创建的，里面包含了许多图形学算法的实现，不仅可以用来提升对算法的理解，对于使用和开发工业级图形软件(比如游戏引擎，DCC软件等)也有巨大的帮助

## 已实现的算法
- 数学库(包含图形学常用的计算，矩阵计算，透视矩阵，正交矩阵，四元数，欧拉角)
- Blinn Phong光照模型
- 天空盒
- 相交检测
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
  - 如果需要构建和调试TG Header Tool，需要安装Clang。参考[官方教程](https://clang.llvm.org/docs/LibASTMatchersTutorial.html)编译LLVM项目，再运行`cmake --install <build dir> --config <buildtype> --prefix <TGGraphicLab dir>/install/<buildtype>`安装到对应目录下
### 编译&调试
- 使用Visual Studio
  - 运行BuildTools/BuildAll.py，传入构建配置参数，完成后会在build/TGGraphicLab目录下的对应构建配置文件夹中生成sln文件和编译好的TGRenderer。如果需要使用VS调试，打开sln文件即可。注意，在VS里运行时，配置要改为运行BuildAll.py脚本时传入的配置，比如传入Debug，那么在VS里Run Configuration就设置为Debug
- 使用Clion
  - 运行BuildTools/BuildDependencies.py，再用clion打开项目即可
- 运行TGRenderer时需要设置的环境变量： `MIMALLOC_DISABLE_REDIRECT=1` `MIMALLOC_SHOW_ERRORS=1` `MIMALLOC_SHOW_STATS=1` 
`MIMALLOC_VERBOSE=1`

## 参考资料
  - [LearnOpenGL](https://learnopengl.com/)
  - [PlanetChili](https://github.com/planetchili/hw3d)
  - [Ray tracing in one weekend](https://raytracing.github.io/)
