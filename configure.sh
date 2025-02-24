curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
source $HOME/.cargo/env
npm install -D netlify-plugin-rust-cache
cargo install mdbook mdbook-katex mdbook-admonish mdbook-embedify --jobs=4
mdbook-admonish install
mdbook build