use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Operator {
    Plus, Minus, Star, Slash, Modulo,
    CColon
}

impl Operator {
    #[inline]
    pub const fn can_infix(&self) -> bool {
        true // there's no purely infix ops rn
    }
    
    #[inline]
    pub const fn can_prefix(&self) -> bool {
        matches!(self, Self::Plus | Self::Minus)
    }
    
    #[inline]
    pub const fn binding_power(&self) -> usize {
        match self {
            Self::CColon => 10,
            Self::Plus | Self::Minus => 20,
            Self::Star | Self::Slash | Self::Modulo => 30,
        }
    }
}

impl fmt::Display for Operator {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Plus => write!(f, "+"),
            Self::Minus => write!(f, "-"),
            Self::Star => write!(f, "*"),
            Self::Slash => write!(f, "/"),
            Self::Modulo => write!(f, "%"),
            Self::CColon => write!(f, "::"),
        }
    }
}