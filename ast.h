#pragma once

#include "scan.h"

typedef enum exprtype_t_ {
    EX_INT,
    EX_IDENT,
    EX_INFIX
} exprtype_t;

typedef enum stmttype_t_ {
    ST_COMPOUND,
    ST_EXPR
} stmttype_t;

typedef struct expr_t_ expr_t;
typedef struct stmt_t_ stmt_t;

/* EXPRESSIONS */

typedef struct expr_infix_t_ {
    expr_t *left, *right;
    toktype_t op;
} expr_infix_t;

typedef struct expr_t_ {
    exprtype_t type;
    union {
        int integer;
        char *ident;
        expr_infix_t *infix;
    };
} expr_t;

/* STATEMENTS */

typedef struct stmt_compound_t_ {
    stmt_t *statements;
    unsigned int amount;
} stmt_compound_t;

typedef struct stmt_t_ {
    stmttype_t type;
    union {
        stmt_compound_t *compound;
        expr_t *expr;
    };
} stmt_t;

void print_expr(int indent, expr_t *e);
void print_stmt(int indent, stmt_t *s);
