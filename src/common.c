#include "common.h"

#ifdef _WIN32
    char* strndup(const char* s, size_t n) {
        size_t len = strnlen(s, n);
        char* dup = (char*)malloc(len + 1);
        if (dup) {
            memcpy(dup, s, len);
            dup[len] = '\0';
        }
        return dup;
    }
#endif

char* operat_to_str(operat* op) {
    switch (*op) {
        case OP_PLUS:   return "+";
        case OP_MINUS:  return "-";
        case OP_STAR:   return "*";
        case OP_SLASH:  return "/";
        case OP_MODULO: return "%";
    }
}