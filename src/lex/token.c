#include <stdlib.h>
#include "token.h"
#include "../common.h"
#include <inttypes.h>
#include <stdio.h>
#include "../common.h"

IMPL_VEC(tok_t, tok_stream, destroy_tok);

void destroy_tok(tok_t* tok) {
    switch (tok->ty.tag) {
        case TOK_IDENT:
            free(tok->ty.payload.s);
            break;
        default: break;
    }
}

char* format_tok(tok_t* tok) {
    switch (tok->ty.tag) {
        case TOK_INT: {
            size_t needed = snprintf(NULL, 0, "%"PRId64, tok->ty.payload.i) + 1;
            char* buf = malloc(needed);
            if (buf) snprintf(buf, needed, "%"PRId64, tok->ty.payload.i);
            return buf;
        }
        case TOK_IDENT:    return strdup(tok->ty.payload.s);
        case TOK_OP:       return strdup(op_to_str(&tok->ty.payload.op));
        case TOK_LPAREN:   return strdup("(");
        case TOK_RPAREN:   return strdup(")");
        case TOK_LCURLY:   return strdup("{");
        case TOK_RCURLY:   return strdup("}");
        case TOK_COLON:    return strdup(":");
        case TOK_CCOLON:   return strdup("::");
        case TOK_PROC:     return strdup("proc");
        case TOK_FUNC:     return strdup("func");
        case TOK_CALLABLE: return strdup("callable");
    }

    // unreachable
    return strdup("???");
}

char* tok_ty_to_str(tok_ty_t* ty) {
    switch (ty->tag) {
        CASE_TO_STR(TOK_INT);
        CASE_TO_STR(TOK_IDENT);
        CASE_TO_STR(TOK_OP);
        CASE_TO_STR(TOK_LPAREN);
        CASE_TO_STR(TOK_RPAREN);
        CASE_TO_STR(TOK_LCURLY);
        CASE_TO_STR(TOK_RCURLY);
        CASE_TO_STR(TOK_COLON);
        CASE_TO_STR(TOK_CCOLON);
        CASE_TO_STR(TOK_PROC);
        CASE_TO_STR(TOK_FUNC);
        CASE_TO_STR(TOK_CALLABLE);
    }
}
