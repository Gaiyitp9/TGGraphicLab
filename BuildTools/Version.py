import subprocess
import argparse
from pathlib import Path
from datetime import datetime


def get_git_info():
    commit_hash = subprocess.check_output(
        ["git", "rev-parse", "HEAD"],
        text=True
    )
    branch = subprocess.check_output(
        ["git", "branch", "--show-current"],
        text=True
    )
    commit_time = subprocess.check_output(
        ["git", "log", "-1", "--format=%cd", "--date=format:%Y-%m-%d %H:%M:%S %z"],
        text=True
    )
    return commit_hash, branch, commit_time


def write_git_log():
    parser = argparse.ArgumentParser()
    parser.add_argument('build_dir', help="The build directory")
    args = parser.parse_args()

    file_path = Path(args.build_dir) / "GitLog.txt"

    commit_hash, branch, commit_time = get_git_info()
    build_time = datetime.now().astimezone().strftime("%Y-%m-%d %H:%M:%S %z(%Z)")
    with open(file_path, "w") as f:
        f.write(f"commit id: {commit_hash}"
                f"branch: {branch}"
                f"commit time: {commit_time}"
                f"build time: {build_time}")


if __name__ == "__main__":
    write_git_log()
