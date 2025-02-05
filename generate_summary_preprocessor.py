import os, glob

def process_markdown_files():
    """
    遍历脚本目录下的 src 文件夹中的所有 Markdown 文件，
    并自动将文件名设置为文章标题。如果文件中已经有标题，则跳过处理。
    """
    # 获取脚本所在目录
    script_dir = os.getcwd()
    src_dir = os.path.join(script_dir, "src")

    print(f"Starting processing in directory: {src_dir}")

    for root, dirs, files in os.walk(src_dir):
        for file in files:
            if file.endswith(".md"):
                file_path = os.path.join(root, file)
                # 获取文件名（不包括扩展名）
                title = os.path.splitext(file)[0]
                
                if title == "SUMMARY":
                    continue

                # 读取文件内容
                with open(file_path, "r", encoding="utf-8") as f:
                    content = f.read()
                
                # 检查文件内容是否已经有标题
                if not content.lstrip().startswith("# "):
                    # 在文件内容开头插入标题
                    new_content = f"# {title}\n{content}"
                    print(f"Processed: {file_path}")
                else:
                    print(f"Title already exists, skipped: {file_path}")
                    new_content = content  # 保留原内容
                
                # 将内容写回文件
                with open(file_path, "w", encoding="utf-8") as f:
                    f.write(new_content)

def get_md_files(directory):
    # 使用 glob 模块匹配所有 .md 文件
    md_files = [os.path.basename(file) for file in glob.glob(os.path.join(directory, '*.md'))]
    return md_files

def generate_content(directory):
    md_files = get_md_files(directory)
    result = "[简介](./index.md)\n"
    for file in md_files:
        if file != "SUMMARY.md" and file != "index.md":
            result += f"[{file}](./{file})\n"
    return str(result)

def main():
    """
    主函数：生成 SUMMARY.md 文件。
    """
    # 设置内容根目录（通常是当前目录）
    base_dir = os.getcwd()
    
    process_markdown_files()

    # 生成 SUMMARY.md 内容
    generated_content = generate_content(os.path.join(base_dir,'src'))
    summary_content = f"# Summary\n\n{generated_content}" if generated_content else "# Summary"
    
    # 写入 SUMMARY.md 文件
    summary_path = os.path.join(base_dir, 'src', 'SUMMARY.md')
    with open(summary_path, 'w', encoding='utf-8', newline='\n') as f:
        f.write(summary_content)
    
    # print(f"SUMMARY.md generated at: {summary_path}")

if __name__ == '__main__':
    main()