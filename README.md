# 天工图形代码库

## 快速开始

克隆项目
```bash
git clone https://github.com/Gaiyitp9/TGGraphicLab.git
```

运行构建脚本
```bash
python BuildTools/Build.py
```
在install目录下打开TGRenderer启动天工渲染器

## 编译&调试

- CLion
    - 直接使用CLion打开项目目录
- Visual Studio 2026
    - 在项目目录下运行
      ```powershell
      cmake -S . --preset visual-studio
      ```
      完成后打开在build目录下的TGGraphicLab.sln
