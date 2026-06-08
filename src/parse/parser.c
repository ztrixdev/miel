#include "parser.h"
#include "../lex/token.h"

parser_t create_parser(tok_stream_t* toks) {
    return (parser_t){
        .toks = toks,
        .pos = 0
    };
}

void destroy_parser(parser_t* parser) {
    destroy_tok_stream(parser->toks);
    free(parser->toks);
    parser->pos = 0;
}