#include "ast.h"
#include "../common.h"

IMPL_VEC(arena_offset_t, ast_nodes, empty_destroy);

void destroy_ast(ast_t* ast) {
    destroy_ast_nodes(&ast->nodes);
    destroy_arena(&ast->node_arena);
}