# 天工图形代码库<br>
[![GitHub](https://img.shields.io/badge/license-MIT-brightgreen)](https://opensource.org/licenses/MIT)  
- 图形学算法代码库。练习图形学算法和编程技巧
- 参考资料
  - [LearnOpenGL](https://learnopengl.com/)
  - [PlanetChili](https://github.com/planetchili/hw3d)
  - [Ray tracing in one weekend](https://raytracing.github.io/)

## 构建
- 项目部分三方库是仓库的子模块，第一次克隆时运行`git clone --recurse-submodules <url>`；如果已经拉取仓库，运行`git submodule update --init --recursive`获取和更新子模块
- 使用visual studio构建和调试
  - 运行BuildTools/BuildAll.py，在build/TGGraphicLab目录下会生成sln文件，用VS打开构建和调试。注意，build/TGGraphicLab目录下会生成Debug和Release目录，需要哪种配置就打开哪个目录下的sln，不要在VS里修改配置
- 使用clion调试项目
  - 运行BuildTools/BuildDependencies.py，再用clion打开项目进行构建和调试
- 运行TGRenderer时需要设置的环境变量： `MIMALLOC_DISABLE_REDIRECT=1` `MIMALLOC_SHOW_ERRORS=1` `MIMALLOC_SHOW_STATS=1` 
`MIMALLOC_VERBOSE=1`