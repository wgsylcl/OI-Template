import sys
import subprocess
from PySide6.QtCore import QTimer
from PySide6.QtWidgets import QApplication, QDialog, QPushButton, QVBoxLayout, QLabel, QMessageBox
import os
import win32process

workpath = os.path.dirname(os.path.realpath(sys.argv[0]))

class CountdownDialog(QDialog):
    def __init__(self, parent=None, seconds=300):
        super().__init__(parent)
        self.setWindowTitle("Countdown")
        self_layout = QVBoxLayout(self)  # 确保布局设置到对话框上
        self.label = QLabel(f"Waiting for {seconds} seconds, press the button to continue...")
        self_layout.addWidget(self.label)
        self.button = QPushButton("Continue")
        self.button.clicked.connect(self.accept)
        self_layout.addWidget(self.button)
        self.stopbutton = QPushButton("Stop")
        self.stopbutton.clicked.connect(self.closeapp)
        self_layout.addWidget(self.stopbutton)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_countdown)
        self.rest = seconds
        self.timer.start(1000)  # 1 second interval

    def update_countdown(self):
        self.rest -= 1
        self.label.setText(f"Waiting for {self.rest} seconds, press the button to continue...")
        if self.rest <= 0:
            self.timer.stop()
            self.accept()

    def closeapp(self):
        run_git_commands(workpath)
        sys.exit(0)

    def closeEvent(self, event):
        # 创建确认对话框
        reply = QMessageBox.question(self, "确认", "您确定要关闭窗口吗？",
                                     QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if reply == QMessageBox.Yes:
            self.closeapp()
        else:
            event.ignore()  # 忽略关闭事件，保持窗口打开

def run_git_commands(script_dir):
    try:
        # 拉取最新的代码
        subprocess.run(["git", "pull"], check=True, cwd=script_dir)

        # 添加文件
        subprocess.run(["git", "add", "."], check=True, cwd=script_dir)

        # 提交更改
        subprocess.run(["git", "commit", "-m", "upd"], check=True, cwd=script_dir)

        # 推送更改
        subprocess.run(["git", "push"], check=True, cwd=script_dir)
    except subprocess.CalledProcessError as e:
        pass
        # QMessageBox.warning(None, "Error", f"Git error: {e}")

def main_loop():
    script_dir = workpath
    while True:
        run_git_commands(script_dir)
        dialog = CountdownDialog()
        dialog.exec()  # 阻塞直到对话框关闭

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.aboutToQuit.connect(lambda: {
        run_git_commands(workpath)
    })
    win32process.SetProcessShutdownParameters(0x100, 1)
    # 检查 Git 命令是否可用
    try:
        subprocess.run(["git", "--version"], check=True)
    except subprocess.CalledProcessError:
        QMessageBox.critical(None, "Error", "Git is not available on this system.")
        sys.exit(1)
    main_loop()