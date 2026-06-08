pub mod ast;

use crate::lexer::token::{Token, TokenKind};
use crate::common::{Span, Context};
use ast::*;
use codespan_reporting::diagnostic::{Diagnostic, Label};

pub struct Parser<'p> {
    ctx: &'p Context<'p>,
    tokens: &'p [Token<'p>],
    pos: usize,
    next_node_id: usize,
}

impl<'p> Parser<'p> {
    pub fn new(ctx: &'p Context<'p>, tokens: &'p [Token<'p>]) -> Self {
        Self {
            ctx, tokens,
            pos: 0usize,
            next_node_id: 0usize,
        }
    }

    #[inline]
    fn create_node(&mut self, kind: NodeKind, span: Span) -> Node {
        let id = NodeId(self.next_node_id);
        self.next_node_id += 1;
        Node { id, kind, span }
    }
    #[inline]
    fn advance(&mut self) { self.pos += 1 }
    #[inline]
    fn expect(&mut self, expected: TokenKind) -> Result<&Token<'p>, Diagnostic<usize>> {
        match self.tokens.get(self.pos) {
            Some(tok) if tok.kind == expected => {
                self.advance();
                Ok(tok)
            },
            Some(other) => Err(Diagnostic::error()
                .with_message(format!("Unexpected `{}`", other.format(self.ctx.rodeo)))
                .with_labels(vec![
                    Label::primary(self.ctx.source_id, other.span.start..other.span.end)
                        .with_message(format!(
                            "expected `{}`, found `{}`",
                            expected.format(self.ctx.rodeo),
                            other.format(self.ctx.rodeo)
                        ))
                ])),
            None => {
                let span = self.tokens.last()
                    .map(|tok| tok.span.splat_to_end())
                    .unwrap_or(Span::empty(self.ctx.source_id));
                Err(Diagnostic::error()
                    .with_message("Unexpected end of input")
                    .with_labels(vec![
                        Label::primary(self.ctx.source_id, span.start..span.end)
                            .with_message(format!(
                                "expected `{}`, found end of input",
                                expected.format(self.ctx.rodeo)
                            ))
                    ]))
            },
        }
    }
    #[inline]
    fn expect_any_without_advance(&mut self, expected: &str) -> Result<&Token<'p>, Diagnostic<usize>> {
        self.tokens.get(self.pos)
            .ok_or_else(|| {
                let span = self.tokens.last()
                    .map(|tok| tok.span.splat_to_end())
                    .unwrap_or(Span::empty(self.ctx.source_id));
                Diagnostic::error()
                    .with_message("Unexpected end of input")
                    .with_labels(vec![
                        Label::primary(self.ctx.source_id, span.start..span.end)
                            .with_message(format!("expected {expected}, found end of input"))
                    ])
            })
    }

    pub fn parse(&mut self) -> Result<Ast, Diagnostic<usize>> {
        let mut nodes = Vec::new();

        while self.tokens.get(self.pos).is_some() {
            nodes.push(self.parse_expression(0)?);
        }

        return Ok(Ast(nodes.into()))
    }

    fn parse_expression(&mut self, min_bp: usize) -> Result<Node, Diagnostic<usize>> {
        let mut lhs = self.parse_primary()?;
        while let Some(tok) = self.tokens.get(self.pos) {
            match &tok.kind {
                TokenKind::Operator(op) if op.can_infix() => {
                    let bp = {
                        let bp = op.binding_power();
                        if bp < min_bp { break }
                        bp + 1
                    };
                    self.advance();
                    let rhs = self.parse_expression(bp)?;
                    let span = lhs.span.concat(&rhs.span);
                    lhs = self.create_node(
                        NodeKind::BinaryOp {
                            op: *op,
                            lhs: Box::new(lhs),
                            rhs: Box::new(rhs)
                        },
                        span
                    );
                }
                _ => break,
            }
        }
        Ok(lhs)
    }

    fn parse_primary(&mut self) -> Result<Node, Diagnostic<usize>> {
        let tok = *self.expect_any_without_advance("expression")?;
        match &tok.kind {
            TokenKind::IntLit(i) => {
                self.advance();
                Ok(self.create_node(
                    NodeKind::IntLit(*i),
                    tok.span
                ))
            },
            TokenKind::FloatLit(i) => {
                self.advance();
                Ok(self.create_node(
                    NodeKind::FloatLit(*i),
                    tok.span
                ))
            },
            TokenKind::StringLit(i) => {
                self.advance();
                Ok(self.create_node(
                    NodeKind::StringLit(i.to_string()),
                    tok.span
                ))
            },
            TokenKind::Identifier(i) => {
                self.advance();
                Ok(self.create_node(
                    NodeKind::Identifier(*i),
                    tok.span
                ))
            },
            TokenKind::LParen => self.parse_paren(),
            TokenKind::Operator(op) if op.can_prefix() => {
                self.advance();
                let operand = self.parse_expression(0)?;
                let span = tok.span.concat(&operand.span);
                Ok(self.create_node(
                    NodeKind::UnaryOp {
                        op: *op,
                        operand: Box::new(operand)
                    },
                    span
                ))
            },
            other => Err(Diagnostic::error()
                .with_message(format!("Unexpected `{}`", other.format(self.ctx.rodeo)))
                .with_labels(vec![
                    Label::primary(self.ctx.source_id, tok.span.start..tok.span.end)
                        .with_message(format!(
                            "expected expression, found `{}`",
                            other.format(self.ctx.rodeo)
                        ))
                ]))
        }
    }

    fn parse_paren(&mut self) -> Result<Node, Diagnostic<usize>> {
        let mut span = self.expect(TokenKind::LParen)?.span;
        let mut items = Vec::new();
        let mut is_tuple = false;
        while let Some(tok) = self.tokens.get(self.pos) {
            if tok.kind == TokenKind::RParen { break }
            items.push(self.parse_expression(0)?);
            if self.expect(TokenKind::Comma).is_ok() { is_tuple = true }
            else { break }
        }
        span = span.concat(&self.expect(TokenKind::RParen)?.span);
        if is_tuple {
            Ok(self.create_node(
                NodeKind::Tuple(items),
                span,
            ))
        } else if items.is_empty() {
            Ok(self.create_node(
                NodeKind::Unit,
                span,
            ))
        } else {
            Ok(items.into_iter().next().unwrap())
        }
    }
}