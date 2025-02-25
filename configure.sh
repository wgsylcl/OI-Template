# #!/bin/bash

# # 设置 Rust 镜像源
# export RUSTUP_DIST_SERVER="https://rsproxy.cn"
# export RUSTUP_UPDATE_ROOT="https://rsproxy.cn/rustup"

# # 检查 Rust 是否已安装
# if ! command -v rustup &> /dev/null; then
#   echo "Installing Rust..."
#   curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
#   source $HOME/.cargo/env
# else
#   rustup default stable
#   echo "Rust is already installed."
# fi

# # 确保 ~/.cargo/bin 在 PATH 中
# export PATH="$HOME/.cargo/bin:$PATH"

# # 安装 mdbook 和相关插件
# echo "Installing mdbook and plugins..."
# cargo install mdbook mdbook-katex mdbook-admonish mdbook-embedify --jobs=4

# # 安装 mdbook-admonish
# echo "Installing mdbook-admonish..."
./linux/mdbook-admonish install

rm ./book.toml
mv ./book.linux.toml ./book.toml

# 构建 mdbook
echo "Building mdbook..."
./linux/mdbook build