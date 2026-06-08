use crate::common::{Operator, Span};
use std::ops::{Deref, DerefMut};

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum TokenKind<'tok> {
    IntLit(i64), FloatLit(f64),
    StringLit(&'tok str),
    Identifier(lasso::Spur),
    Operator(Operator),
    LParen, RParen, LCurly, RCurly,
    Colon, Comma,
    KwProc, KwFunc, KwCallable
}

impl<'tok> TokenKind<'tok> {
    pub fn format(&self, rodeo: &lasso::Rodeo) -> String {
        match self {
            Self::IntLit(i) => i.to_string(),
            Self::FloatLit(i) => i.to_string(),
            Self::StringLit(i) => format!("\"{i}\""),
            Self::Identifier(s) => rodeo.resolve(s).to_string(),
            Self::Operator(o) => o.to_string(),
            Self::LParen => "(".to_string(),
            Self::RParen => ")".to_string(),
            Self::LCurly => "{".to_string(),
            Self::RCurly => "}".to_string(),
            Self::Colon => ":".to_string(),
            Self::Comma => ",".to_string(),
            Self::KwProc => "proc".to_string(),
            Self::KwFunc => "func".to_string(),
            Self::KwCallable => "callable".to_string(),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Token<'tok> {
    pub kind: TokenKind<'tok>,
    pub span: Span
}

impl<'tok> Deref for Token<'tok> {
    type Target = TokenKind<'tok>;
    fn deref(&self) -> &Self::Target {
        &self.kind
    }
}

impl<'tok> DerefMut for Token<'tok> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.kind
    }
}