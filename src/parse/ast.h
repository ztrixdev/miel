#ifndef IVO_AST_H
#define IVO_AST_H

#include "../common.h"
#include "../arena.h"

typedef struct ast_node ast_node_t;
typedef size_t node_id_t;

typedef struct ast_node_ty {
    enum {
        AST_INT_LIT, AST_IDENT,
        AST_BIN_OP, AST_UNARY_OP
    } tag;
    union {
        long long int_lit;
        char* ident;
        struct {
            ast_node_t* lhs;
            operator_t op;
            ast_node_t* rhs;
        } bin_op;
        struct {
            operator_t op;
            ast_node_t* operand;
        } unary_op;
    } payload;
} ast_node_ty_t;

typedef struct ast_node {
    span_t span;
    ast_node_ty_t ty;
} ast_node_t;

DECL_VEC(arena_offset_t, ast_nodes);

typedef struct ast {
    node_id_t id;
    arena_t node_arena;
    ast_nodes_t nodes;
} ast_t;

void destroy_ast(ast_t* ast);

#endif
