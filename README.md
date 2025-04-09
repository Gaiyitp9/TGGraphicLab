# 天工图形代码库<br>
[![GitHub](https://img.shields.io/badge/license-MIT-brightgreen)](https://opensource.org/licenses/MIT)  
- 图形学算法代码库。练习图形学算法和编程技巧
- 参考资料
  - [LearnOpenGL](https://learnopengl.com/)
  - [PlanetChili](https://github.com/planetchili/hw3d)

## 构建
- `git submodule update --init`获取最新的三方库
- 运行BuildTools/BuildDependencies.py构建三方库，完成后再构建TGGraphicLab
- 运行时需要设置的环境变量： `MIMALLOC_DISABLE_REDIRECT=1` `MIMALLOC_SHOW_ERRORS=1` `MIMALLOC_SHOW_STATS=1` 
`MIMALLOC_VERBOSE=1`