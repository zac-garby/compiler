#pragma once

#include "ast.h"

typedef struct parse_error_t_ {
    char *message;
} parse_error_t;

typedef struct parser_t_ {
    scanner_t *s;
    parse_error_t *err;
} parser_t;

int parse_expr(parser_t *p, expr_t *e);
int parse_stmt(parser_t *p, stmt_t *s);
