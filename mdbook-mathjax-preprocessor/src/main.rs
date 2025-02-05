use fancy_regex::Regex;
use mdbook::preprocess::{Preprocessor, PreprocessorContext};
use mdbook::BookItem;
use once_cell::sync::Lazy;
use pulldown_cmark::{Event, Parser};
use pulldown_cmark_to_cmark::cmark;

use std::fs::OpenOptions;
use std::io::{Result, Write};

/// 将调试信息写入到指定的日志文件中
///
/// # 参数
/// - `file_path`: 日志文件的路径
/// - `message`: 要写入的调试信息
///
/// # 示例
/// ```rust
/// log_to_file("app.log", "This is a debug message").unwrap();
/// ```
pub fn log_to_file(file_path: &str, message: &str) -> Result<()> {
    // 打开文件（追加模式），如果文件不存在则创建
    let mut file = OpenOptions::new()
        .create(true)
        .append(true)
        .open(file_path)?;

    // 写入调试信息
    writeln!(file, "{}", message)?;

    Ok(())
}

#[derive(Default)]
struct MathjaxPreprocessor;

static INLINE_MATH_RE: Lazy<Regex> = Lazy::new(|| {
    Regex::new(r#"(?x)
        (?<!\\)       # 确保起始$未被转义
        (?<!\$)       # 前面没有$（排除块公式起始）
        \$            # 起始$
        (             # 捕获公式内容
            (?:\\\$|\\[^$]|[^$])+?  # 允许任意字符（包括< >）
        )
        (?<!\\)       # 确保结束$未被转义
        \$            # 结束$
        (?!\$)        # 后面没有$（排除块公式结束）
    "#).unwrap()
});


impl Preprocessor for MathjaxPreprocessor {
    fn name(&self) -> &str {
        "mathjax"
    }

    fn run(&self, _ctx: &PreprocessorContext, mut book: mdbook::book::Book) -> mdbook::errors::Result<mdbook::book::Book> {
        book.for_each_mut(|item| {
            if let BookItem::Chapter(chapter) = item {
                let mut buf = String::new();
                let parser = Parser::new(&chapter.content);
                let mut events = Vec::new();

                for event in parser {
                    if let Event::Text(text) = event {
                        let processed = replace_inline_math(&text);
                        events.push(Event::Text(processed.into()));
                    } else {
                        events.push(event);
                    }
                }

                cmark(events.into_iter(), &mut buf).unwrap();
                chapter.content = buf;
            }
        });
        Ok(book)
    }

    fn supports_renderer(&self, renderer: &str) -> bool {
        renderer == "html"
    }
}

fn replace_inline_math(text: &str) -> String {
    INLINE_MATH_RE.replace_all(text, |caps: &fancy_regex::Captures| {
        let content = caps.get(1).map(|m| m.as_str()).unwrap_or("");
        // println!("[DEBUG] Matched inline formula: {:?}", content);

        // 处理转义字符
        let cleaned = content.replace(r"\$", "$").replace(r"\\", r"\");
        format!(r"\\({}\\)", cleaned)
    }).to_string()
}

fn main() -> mdbook::errors::Result<()> {
    use mdbook::preprocess::CmdPreprocessor;

    let preprocessor = MathjaxPreprocessor::default();

    if std::env::args().any(|arg| arg == "supports") {
        let renderer = std::env::args().nth(2).unwrap();
        println!("{}", preprocessor.supports_renderer(&renderer));
        Ok(())
    } else {
        let (ctx, book) = CmdPreprocessor::parse_input(std::io::stdin())?;
        let processed_book = preprocessor.run(&ctx, book)?;
        serde_json::to_writer(std::io::stdout(), &processed_book)?;
        Ok(())
    }
}
