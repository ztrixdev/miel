#ifndef IVO_TOKEN_H
#define IVO_TOKEN_H

#include <stddef.h>
#include "../common.h"

// todo: add string interning for tokens
typedef struct tok_ty {
    enum {
        TOK_INT, TOK_IDENT,
        TOK_OP,
        TOK_LPAREN, TOK_RPAREN,
        TOK_LCURLY, TOK_RCURLY,
        TOK_COLON, TOK_CCOLON,
        TOK_PROC, TOK_FUNC,
        TOK_CALLABLE
    } tag;
    union {
        long long i;
        char* s;
        operator_t op;
    } payload;
} tok_ty_t;

typedef struct tok {
    span_t span;
    tok_ty_t ty;
} tok_t;

DECL_VEC(tok_t, tok_stream);
void destroy_tok(tok_t* tok);
char* format_tok(tok_t* tok);
char* tok_ty_to_str(tok_ty_t* ty);

#endif
