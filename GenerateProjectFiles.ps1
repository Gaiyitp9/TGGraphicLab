# 构建TGGraphicLab

# 配置构建目录和安装目录
$sourceDir = "$PWD"
$buildDir = "$PWD\build"
$installDir = "$PWD\install"

Write-Host "Source directory: $sourceDir"
Write-Host "Build directory: $buildDir"
Write-Host "Install directory: $installDir"

# 确保构建目录存在
if (-Not (Test-Path -Path $buildDir))
{
   New-Item -ItemType Directory -Path $buildDir
}

# CMake配置参数
$generator = "Visual Studio 17 2022"
$cmakeOptions = @(
   "-DCMAKE_INSTALL_PREFIX=$installDir"
   "-DCMAKE_VERBOSE_MAKEFILE=OFF"
   "-DSPDLOG_USE_STD_FORMAT:BOOL=ON"
   "-DSPDLOG_INSTALL:BOOL=ON"
   "-DMI_BUILD_TESTS:BOOL=OFF"
)

function GenerateProjectFiles
{
   param
   (
      [string]$buildType
   )

   # 运行 CMake 配置命令
   Write-Host "Configuring the $buildType project with CMake..."
   $cmakeCmd = "cmake -DCMAKE_BUILD_TYPE=$buildType $($cmakeOptions -join ' ') -G '$generator'" +
      "-S $sourceDir -B $buildDir"
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
}

# 构建Debug版本
GenerateProjectFiles "Debug"
# 构建Release版本
GenerateProjectFiles "Release"

Write-Host "Configuration and build complete!"
