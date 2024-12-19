# 构建TGGraphicLab依赖的三方库
# 配置构建目录和安装目录
$sourceDir = "$PWD\ThirdParty"
$buildDir = "$PWD\build"
$installDir = "$PWD\install"

Write-Host "Source directory: $sourceDir"
Write-Host "Build directory: $buildDir"
Write-Host "Install directory: $installDir"

# CMake 配置参数
$generator = "Visual Studio 17 2022"
$buildType = "Debug" # 构建类型（Debug, Release, RelWithDebInfo, MinSizeRel）
$cmakeOptions = @(
   "-DCMAKE_BUILD_TYPE=$buildType",
   "-DCMAKE_INSTALL_PREFIX=$installDir"
   "-DCMAKE_VERBOSE_MAKEFILE=OFF"
   "-DSPDLOG_USE_STD_FORMAT:BOOL=ON"
   "-DSPDLOG_INSTALL:BOOL=ON"
)

# 确保构建目录存在
if (-Not (Test-Path -Path $buildDir)) 
{
   New-Item -ItemType Directory -Path $buildDir
}

# 运行 CMake 配置命令
Write-Host "Configuring the project with CMake..."
$cmakeCmd = "cmake $($cmakeOptions -join ' ') -G '$generator' -S $sourceDir -B $buildDir"
Write-Host "Running: $cmakeCmd"
Invoke-Expression $cmakeCmd

# 运行 CMake 构建命令
Write-Host "Building the project..."
$buildCmd = "cmake --build $buildDir --config $buildType"
Write-Host "Running: $buildCmd"
Invoke-Expression $buildCmd

# 安装
Write-Host "Installing the project to $installDir..."
$installCmd = "cmake --install $buildDir --config $buildType"
Write-Host "Running: $installCmd"
Invoke-Expression $installCmd

Write-Host "Build and configuration complete!"
