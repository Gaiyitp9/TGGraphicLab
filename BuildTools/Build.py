from pathlib import Path
import subprocess
import shlex


def run_subprocess(cmd):
    cmd_str = shlex.join(cmd)
    print(f"Running: {cmd_str}")
    with subprocess.Popen(cmd,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT,
                          text=True,
                          encoding="utf-8") as process:
        if process.stdout is not None:
            for line in process.stdout:
                print(line.strip())

        return_code = process.wait()
        stdout, stderr = process.communicate()
        if return_code != 0:
            print("run subprocess failed")
            print(f"return code: {return_code}")
            print(f"stderr: {stderr}")
            print(f"stdout: {stdout}")


def main():
    root_dir = Path(__file__).parent.parent.resolve()
    source_dir = root_dir

    configure_preset = "ninja-multi-config"
    build_preset = "ninja-multi-config-release"

    # 运行CMake配置命令
    configure_cmd = ["cmake", "-S", str(source_dir), "--preset", configure_preset]
    run_subprocess(configure_cmd)

    # 运行CMake构建命令
    build_cmd = ["cmake", "--build", "--preset", build_preset]
    run_subprocess(build_cmd)

    # 安装
    install_cmd = ["cmake", "--build", "--preset", build_preset, "--target", "install"]
    run_subprocess(install_cmd)


if __name__ == "__main__":
    main()
