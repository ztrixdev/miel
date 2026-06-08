#ifndef IVO_PARSER_H
#define IVO_PARSER_H

#include "../lex/token.h"

typedef struct parser {
    tok_stream_t* toks;
    size_t pos;
} parser_t;

parser_t create_parser(tok_stream_t* toks);
void destroy_parser(parser_t* parser);

#endif