#!/bin/bash

# 设置 Rust 镜像源
export RUSTUP_DIST_SERVER="https://rsproxy.cn"
export RUSTUP_UPDATE_ROOT="https://rsproxy.cn/rustup"

# 检查 Rust 是否已安装
if ! command -v rustup &> /dev/null; then
  echo "Installing Rust..."
  curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
  source $HOME/.cargo/env
else
  echo "Rust is already installed."
  rustup default stable
fi

# 安装 mdbook 和相关插件
cargo install mdbook mdbook-katex mdbook-admonish mdbook-embedify --jobs=4

# 安装 mdbook-admonish
mdbook-admonish install

# 构建 mdbook
mdbook build