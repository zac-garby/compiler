#pragma once

#include "ast.h"
#include "scan.h"

typedef struct parse_error_t_ {
    char *message;
} parse_error_t;

typedef struct parser_t_ {
    scanner_t *s;
    token_t cur;
    parse_error_t err;
} parser_t;

int parser_advance(parser_t *p);

int parse_expr(parser_t *p, expr_t *e);
int parse_stmt(parser_t *p, stmt_t *s);

int parse_integer(parser_t *p, expr_t *e);
int parse_ident(parser_t *p, expr_t *e);
int parse_assignment(parser_t *p, expr_t *e);
int parse_term(parser_t *p, expr_t *e);
int parse_factor(parser_t *p, expr_t *e);
int parse_call(parser_t *p, expr_t *e);
int parse_atom(parser_t *p, expr_t *e);

int parse_compound(parser_t *p, stmt_t *s);
