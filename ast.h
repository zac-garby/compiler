#pragma once

typedef enum exprtype_t_ {
    EX_INT,
    EX_IDENT,
    EX_INFIX
} exprtype_t;

typedef enum stmttype_t_ {
    ST_COMPOUND,
    ST_EXPR
} stmttype_t;
