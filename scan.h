#pragma once

typedef enum toktype_t_ {
    T_EOF,
    T_INT,
    T_PLUS,
    T_MINUS,
    T_TIMES,
    T_DIVIDE,
    T_LPAREN,
    T_RPAREN,
    T_IDENT,
    T_ASSIGN,
    T_SEMI,
    T_COMMA,
} toktype_t;

typedef struct token_t {
    toktype_t type;
    char *lexeme;
} token_t;

typedef struct scanner_t {
    char *cursor;
} scanner_t;

int scanner_next(scanner_t *s, token_t *t);
char *tok_type(toktype_t type);
