import os
import subprocess
from pathlib import Path
import shlex


def run_subprocess(cmd):
    cmd_str = shlex.join(cmd)
    print(f"Running: {cmd_str}")
    with subprocess.Popen(cmd,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT,
                          text=True,
                          encoding='utf-8') as process:
        for line in process.stdout:
            print(line.strip())

        return_code = process.wait()
        stdout, stderr = process.communicate()
        if return_code != 0:
            print("run subprocess failed")
            print(f"return code: {return_code}")
            print(f"stderr: {stderr}")
            print(f"stdout: {stdout}")


def build_library(local_source_dir, local_build_dir, local_generator, local_cmake_options, local_install_dir):
    try:
        local_source_dir = os.fspath(local_source_dir)

        local_build_dir = os.fspath(local_build_dir)

        local_install_dir = os.fspath(local_install_dir)
    except TypeError as e:
        print(e)

    # 运行CMake配置命令
    print("Configuring the project...")
    configure_cmd = ["cmake", "-S", local_source_dir, "-B", local_build_dir, "-G", local_generator,
                     "-DCMAKE_VERBOSE_MAKEFILE=OFF"]
    configure_cmd.extend(local_cmake_options)
    run_subprocess(configure_cmd)

    # 运行CMake构建命令
    print("Building the project...")
    build_cmd = ["cmake", "--build", local_build_dir, "--config", "Debug"]
    run_subprocess(build_cmd)
    build_cmd = ["cmake", "--build", local_build_dir, "--config", "RelWithDebInfo"]
    run_subprocess(build_cmd)
    build_cmd = ["cmake", "--build", local_build_dir, "--config", "Release"]
    run_subprocess(build_cmd)
    build_cmd = ["cmake", "--build", local_build_dir, "--config", "MinSizeRel"]
    run_subprocess(build_cmd)

    # 安装
    print(f"Installing the project to {local_install_dir}...")
    install_dir_debug = str(Path(local_install_dir) / "Debug")
    install_cmd = ["cmake", "--install", local_build_dir, "--config", "Debug", "--prefix", install_dir_debug]
    run_subprocess(install_cmd)
    install_dir_relWithDebInfo = str(Path(local_install_dir) / "RelWithDebInfo")
    install_cmd = ["cmake", "--install", local_build_dir, "--config", "RelWithDebInfo", "--prefix", install_dir_relWithDebInfo]
    run_subprocess(install_cmd)
    install_dir_release = str(Path(local_install_dir) / "Release")
    install_cmd = ["cmake", "--install", local_build_dir, "--config", "Release", "--prefix", install_dir_release]
    run_subprocess(install_cmd)
    install_dir_minSizeRel = str(Path(local_install_dir) / "MinSizeRel")
    install_cmd = ["cmake", "--install", local_build_dir, "--config", "MinSizeRel", "--prefix", install_dir_minSizeRel]
    run_subprocess(install_cmd)


def build_dependencies():
    root_dir = Path(__file__).parent.parent.resolve()

    install_dir = root_dir / "install"
    print(f"Install directory: {install_dir}")

    generator = "Visual Studio 18 2026"
    print(f"Generator: {generator}")

    source_dir = root_dir / "ThirdParty" / "mimalloc"
    build_dir = root_dir / "build" / "mimalloc"
    cmake_options = ["-DMI_BUILD_TESTS=OFF"]
    build_library(source_dir, build_dir, generator, cmake_options, install_dir)

    source_dir = root_dir / "ThirdParty" / "spdlog"
    build_dir = root_dir / "build" / "spdlog"
    cmake_options = ["-DSPDLOG_USE_STD_FORMAT=ON", "-DSPDLOG_INSTALL=ON"]
    build_library(source_dir, build_dir, generator, cmake_options, install_dir)

    source_dir = root_dir / "ThirdParty" / "glad"
    build_dir = root_dir / "build" / "glad"
    cmake_options = []
    build_library(source_dir, build_dir, generator, cmake_options, install_dir)

    source_dir = root_dir / "ThirdParty" / "glm"
    build_dir = root_dir / "build" / "glm"
    cmake_options = ["-DGLM_BUILD_TESTS=OFF", "-DBUILD_SHARED_LIBS=OFF"]
    build_library(source_dir, build_dir, generator, cmake_options, install_dir)

    print("Configuration and generation completed")


if __name__ == "__main__":
    build_dependencies()
