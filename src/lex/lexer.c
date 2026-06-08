#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "lexer.h"
#include "token.h"
#include "../common.h"

result_t tokenize(const char* src) {
    tok_stream_t* toks = calloc(1, sizeof(tok_stream_t));
    if (!toks) {
        #ifdef ENOMEM
            if (errno == ENOMEM) SIMPLE_ERR("Out of memory");
            else SIMPLE_ERR(strerror(errno));
        #else
            SIMPLE_ERR(strerror(errno));
        #endif
    }

    if (src) {
        size_t src_len = strlen(src);
        size_t i = 0;
        while (i < src_len) {
            char curr_ch = src[i++];
            if (isspace(curr_ch)) continue;
            else if (isdigit(curr_ch)) {
                size_t start = i - 1;
                while (i < src_len && (isdigit(src[i]) || src[i] == '_')) i++;
                size_t len = i - start;
                char* str_int = strndup(src + start, len);
                if (!str_int) {
                    destroy_tok_stream(toks);
                    #ifdef ENOMEM
                        if (errno == ENOMEM) SIMPLE_ERR("Out of memory");
                        else SIMPLE_ERR(strerror(errno));
                    #else
                        SIMPLE_ERR(strerror(errno));
                    #endif
                }
                long long parsed_int = atoll(str_int);
                free(str_int);
                push_to_tok_stream(
                    toks,
                    (tok_t){
                        .span = { .start = start, .end = start + len },
                        .ty = {
                            .tag = TOK_INT,
                            .payload = { .i = parsed_int }
                        }
                    }
                );
            } else if (isalpha(curr_ch) || curr_ch == '_') {
                size_t start = i - 1;
                while (i < src_len && (isalnum(src[i]) || src[i] == '_')) i++;
                size_t len = i - start;
                char* s = strndup(src + start, len);
                if (!s) {
                    destroy_tok_stream(toks);
                    #ifdef ENOMEM
                        if (errno == ENOMEM) SIMPLE_ERR("Out of memory");
                        else SIMPLE_ERR(strerror(errno));
                    #else
                        SIMPLE_ERR(strerror(errno));
                    #endif
                }
                tok_ty_t ty;
                if (STREQ(s, "proc")) {
                    ty = (tok_ty_t){
                        .tag = TOK_PROC,
                        .payload = {0}
                    };
                } else if (STREQ(s, "func")) {
                    ty = (tok_ty_t){
                        .tag = TOK_FUNC,
                        .payload = {0}
                    };
                } else if (STREQ(s, "callable")) {
                    ty = (tok_ty_t){
                        .tag = TOK_CALLABLE,
                        .payload = {0}
                    };
                } else {
                    ty = (tok_ty_t){
                        .tag = TOK_IDENT,
                        .payload = { .s = s }
                    };
                }
                push_to_tok_stream(
                    toks,
                    (tok_t){
                        .span = { .start = start, .end = start + len },
                        .ty = ty
                    }
                );
            } else {
                switch (curr_ch) {
                    case '+': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_OP,
                                    .payload = { .op = OP_PLUS }
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case '-': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_OP,
                                    .payload = { .op = OP_MINUS }
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case '*': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_OP,
                                    .payload = { .op = OP_STAR }
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case '/': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_OP,
                                    .payload = { .op = OP_SLASH }
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case '%': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_OP,
                                    .payload = { .op = OP_MODULO }
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case '(': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_LPAREN,
                                    .payload = {0}
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case ')': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_RPAREN,
                                    .payload = {0}
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case '{': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_LCURLY,
                                    .payload = {0}
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case '}': {
                        result_t push_result = push_to_tok_stream(
                            toks,
                            (tok_t){
                                .span = { .start = i - 1, .end = i },
                                .ty = {
                                    .tag = TOK_RCURLY,
                                    .payload = {0}
                                }
                            }
                        );
                        if (!push_result.is_ok) {
                            destroy_tok_stream(toks);
                            return push_result;
                        }
                        break;
                    }
                    case ':': {
                        if (i + 1 < src_len && src[i] == ':') {
                            i++;
                            result_t push_result = push_to_tok_stream(
                                toks,
                                (tok_t){
                                    .span = { .start = i - 2, .end = i },
                                    .ty = {
                                        .tag = TOK_CCOLON,
                                        .payload = {0}
                                    }
                                }
                            );
                            if (!push_result.is_ok) {
                                destroy_tok_stream(toks);
                                return push_result;
                            }
                        } else {
                            result_t push_result = push_to_tok_stream(
                                toks,
                                (tok_t){
                                    .span = { .start = i - 1, .end = i },
                                    .ty = {
                                        .tag = TOK_COLON,
                                        .payload = {0}
                                    }
                                }
                            );
                            if (!push_result.is_ok) {
                                destroy_tok_stream(toks);
                                return push_result;
                            }
                        }
                        break;
                    }
                    default: {
                        destroy_tok_stream(toks);
                        size_t needed = strlen("unrecognized char ``") + 2;
                        char* buf = malloc(needed);
                        if (buf) snprintf(buf, needed, "unrecognized char `%c`", curr_ch);
                        return (result_t){
                            .is_ok = false,
                            .payload = { .err = buf }
                        };
                    }
                }
            }
        }
    }

    return (result_t){
        .is_ok = true,
        .payload = { .ok = toks }
    };
}
