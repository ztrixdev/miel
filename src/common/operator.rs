use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Operator {
    Plus, Minus, Star, Slash, Modulo
}

impl fmt::Display for Operator {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Plus => write!(f, "+"),
            Self::Minus => write!(f, "-"),
            Self::Star => write!(f, "*"),
            Self::Slash => write!(f, "/"),
            Self::Modulo => write!(f, "%"),
        }
    }
}