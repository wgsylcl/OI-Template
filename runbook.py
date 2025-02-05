import sys
import subprocess

def main():
    # 检查是否提供了命令行参数
    if len(sys.argv) < 2:
        print("Usage: python runbook.py <mdbook_command> [args...]")
        sys.exit(1)

    # 运行 generate_summary_preprocessor.py
    print("Running generate_summary_preprocessor.py...")
    subprocess.run(["python", "generate_summary_preprocessor.py"])

    # 获取命令行参数并执行 mdbook 命令
    mdbook_command = sys.argv[1:]  # 获取所有参数，从第一个参数开始
    print(f"Executing mdbook command: {' '.join(mdbook_command)}")
    subprocess.run(["mdbook"] + mdbook_command)

if __name__ == "__main__":
    main()