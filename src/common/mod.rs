mod operator;
mod span;
mod ctx;

pub use operator::*;
pub use span::*;
pub use ctx::*;

pub type Diag = codespan_reporting::diagnostic::Diagnostic<usize>;
pub type Label = codespan_reporting::diagnostic::Label<usize>;