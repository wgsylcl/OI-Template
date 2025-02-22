curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
source $HOME/.cargo/env
cargo install mdbook
cargo install mdbook-katex
cargo install mdbook-admonish
cargo install mdbook-embedify
mdbook-admonish install
mdbook build