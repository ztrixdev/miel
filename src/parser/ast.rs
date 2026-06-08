use crate::common::{Operator, Span};
use std::sync::Arc;

#[derive(Debug, Clone, PartialEq)]
pub struct Ast(pub Arc<[Node]>);

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct NodeId(pub usize);

#[derive(Debug, Clone, PartialEq)]
pub enum NodeKind {
    IntLit(i64), FloatLit(f64),
    StringLit(String),
    Identifier(lasso::Spur),
    Unit,
    BinaryOp {
        op: Operator,
        lhs: Box<Node>,
        rhs: Box<Node>,
    },
    UnaryOp {
        op: Operator,
        operand: Box<Node>,
    },
    Tuple(Vec<Node>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct Node {
    pub id: NodeId,
    pub kind: NodeKind,
    pub span: Span,
}