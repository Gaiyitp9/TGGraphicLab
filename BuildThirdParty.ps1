# 构建TGGraphicLab三方库

$installDir = "$PWD\install"
Write-Host "Install directory: $installDir"

$generator = "Visual Studio 17 2022"
Write-Host "Generator: $generator"

function BuildLibrary
{
   param
   (
      [string]$buildType,
      [string]$sourceDir,
      [string]$buildDir,
      [string[]]$cmakeOptions = @()
   )

   # 确保构建目录存在
   if (-Not (Test-Path -Path $buildDir))
   {
      New-Item -ItemType Directory -Path $buildDir
   }

   # 运行CMake配置命令
   $cmakeCmd = "cmake -G '$generator' " +
               "-DCMAKE_BUILD_TYPE=$buildType " +
               "-DCMAKE_INSTALL_PREFIX=$installDir " +
               "-DCMAKE_VERBOSE_MAKEFILE=OFF " +
               "$($cmakeOptions -join ' ') " +
               "-S $sourceDir -B $buildDir"
   Write-Host "Running: $cmakeCmd"
   Invoke-Expression $cmakeCmd
   # 运行CMake构建命令
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

$sourceDir = "$PWD\ThirdParty\mimalloc"
$buildDir = "$PWD\build\mimalloc"
$cmakeOptions = @(
   "-DMI_BUILD_TESTS:BOOL=OFF"
)
BuildLibrary "Debug" $sourceDir $buildDir $cmakeOptions
BuildLibrary "Release" $sourceDir $buildDir $cmakeOptions

$sourceDir = "$PWD\ThirdParty\spdlog"
$buildDir = "$PWD\build\spdlog"
$cmakeOptions = @(
   "-DSPDLOG_USE_STD_FORMAT:BOOL=ON"
   "-DSPDLOG_INSTALL:BOOL=ON"
)
BuildLibrary "Debug" $sourceDir $buildDir $cmakeOptions
BuildLibrary "Release" $sourceDir $buildDir $cmakeOptions

$sourceDir = "$PWD\ThirdParty\PowerVR"
$buildDir = "$PWD\build\PowerVR"
BuildLibrary "Debug" $sourceDir $buildDir
BuildLibrary "Release" $sourceDir $buildDir

$sourceDir = "$PWD\ThirdParty\glm"
$buildDir = "$PWD\build\glm"
$cmakeOptions = @(
   "-DGLM_BUILD_TESTS:BOOL=OFF"
   "-DBUILD_SHARED_LIBS:BOOL=OFF"
)
BuildLibrary "Debug" $sourceDir $buildDir
BuildLibrary "Release" $sourceDir $buildDir

Write-Host "Configuration and generation completed"
