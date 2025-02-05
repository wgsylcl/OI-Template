curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
source $HOME/.cargo/env
cargo install mdbook
cargo install mdbook-toc
cd mdbook-mathjax-preprocessor
cargo build --release
cd ..
python runbook.py build