import subprocess
import os

# 获取当前Python脚本的绝对路径
script_dir = os.path.dirname(os.path.abspath(__file__))

# 构建PowerShell脚本的完整路径
ps1_script_path = os.path.join(script_dir, 'pushcode.ps1')

# 使用subprocess调用PowerShell执行脚本
# 注意：在Windows系统上，PowerShell的调用方式可能需要调整
subprocess.run(['powershell', '-ExecutionPolicy', 'Bypass', ps1_script_path], check=True)