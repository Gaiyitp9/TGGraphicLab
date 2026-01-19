from pathlib import Path
import os
from BuildDependencies import build_dependencies, run_subprocess


def build_project(build_type):
    root_dir = Path(__file__).parent.parent.resolve()
    build_dir = root_dir / "build" / "TGGraphicLab" / build_type
    generator = "Visual Studio 18 2026"

    source_dir = os.fspath(root_dir)
    build_dir = os.fspath(build_dir)
    # 运行CMake配置命令
    print("Configuring the project...")
    configure_cmd = ["cmake", "-S", source_dir, "-B", build_dir, "-G", generator,
                    f"-DCMAKE_BUILD_TYPE={build_type}",
                    "-DCMAKE_VERBOSE_MAKEFILE=OFF"]
    run_subprocess(configure_cmd)

    # 运行CMake构建命令
    print("Building the project...")
    build_cmd = ["cmake", "--build", build_dir, "--config", build_type]
    run_subprocess(build_cmd)


if __name__ == "__main__":
    build_dependencies()
    build_project("Debug")
    build_project("Release")
