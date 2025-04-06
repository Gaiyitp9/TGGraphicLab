import os
import subprocess
from pathlib import Path

def build_library(local_source_dir, local_build_dir, local_generator, build_type,
                  local_cmake_options, local_install_dir):
    os.makedirs(local_build_dir, exist_ok=True)
    # 运行CMake配置命令
    cmake_cmd = ["cmake", "-S", local_source_dir, "-B", local_build_dir, "-G", local_generator,
                f"-DCMAKE_BUILD_TYPE={build_type}",
                f"-DCMAKE_INSTALL_PREFIX={local_install_dir}",
                "-DCMAKE_VERBOSE_MAKEFILE=OFF"]
    cmake_cmd.extend(local_cmake_options)
    cmake_cmd_str = " ".join(cmake_cmd)
    print(f"Running: {cmake_cmd_str}")
    with subprocess.Popen(cmake_cmd, text=True,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT) as process:
        for line in process.stdout:
            print(line.strip())

        return_code = process.wait()
        if return_code != 0:
            raise subprocess.CalledProcessError(return_code, process.args)
    # 运行CMake构建命令
    print("Building the project...")
    build_cmd = ["cmake", "--build", local_build_dir, "--config", build_type]
    build_cmd_str = " ".join(build_cmd)
    print(f"Running: {build_cmd_str}")
    with subprocess.Popen(build_cmd, text=True,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT) as process:
        for line in process.stdout:
            print(line.strip())

        return_code = process.wait()
        if return_code != 0:
            raise subprocess.CalledProcessError(return_code, process.args)
    # 安装
    print(f"Installing the project to {local_install_dir}...")
    install_cmd = ["cmake", "--install", local_build_dir, "--config", build_type]
    install_cmd_str = " ".join(install_cmd)
    print(f"Running: {install_cmd_str}")
    with subprocess.Popen(install_cmd, text=True,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT) as process:
        for line in process.stdout:
            print(line.strip())

        return_code = process.wait()
        if return_code != 0:
            raise subprocess.CalledProcessError(return_code, process.args)


if __name__ == "__main__":
    os.chdir("..")

    install_dir = Path(os.getcwd()) / "install"
    print(f"Install directory: {install_dir}")

    generator = "Visual Studio 17 2022"
    print(f"Generator: {generator}")

    source_dir = str(Path(os.getcwd()) / "ThirdParty" / "mimalloc")
    build_dir = str(Path(os.getcwd()) / "build" / "mimalloc")
    cmake_options = ["-DMI_BUILD_TESTS=OFF"]
    build_library(source_dir, build_dir, generator, "Debug", cmake_options, install_dir)
    build_library(source_dir, build_dir, generator, "Release", cmake_options, install_dir)

    source_dir = str(Path(os.getcwd()) / "ThirdParty" / "spdlog")
    build_dir = str(Path(os.getcwd()) / "build" / "spdlog")
    cmake_options = ["-DSPDLOG_USE_STD_FORMAT=ON", "-DSPDLOG_INSTALL=ON"]
    build_library(source_dir, build_dir, generator, "Debug", cmake_options, install_dir)
    build_library(source_dir, build_dir, generator, "Release", cmake_options, install_dir)

    source_dir = str(Path(os.getcwd()) / "ThirdParty" / "PowerVR")
    build_dir = str(Path(os.getcwd()) / "build" / "PowerVR")
    cmake_options = []
    build_library(source_dir, build_dir, generator, "Debug", cmake_options, install_dir)
    build_library(source_dir, build_dir, generator, "Release", cmake_options, install_dir)

    source_dir = str(Path(os.getcwd()) / "ThirdParty" / "glm")
    build_dir = str(Path(os.getcwd()) / "build" / "glm")
    cmake_options = ["-DGLM_BUILD_TESTS=OFF", "-DBUILD_SHARED_LIBS=OFF"]
    build_library(source_dir, build_dir, generator, "Debug", cmake_options, install_dir)
    build_library(source_dir, build_dir, generator, "Release", cmake_options, install_dir)

    print("Configuration and generation completed")

