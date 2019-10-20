#ifndef SCAN_H_
#define SCAN_H_

#define T_EOF       0
#define T_INT       1
#define T_PLUS      2
#define T_MINUS     3
#define T_LPAREN    4
#define T_RPAREN    5

typedef struct token_t {
    int type;
    char *lexeme;
} token_t;

typedef struct scanner_t {
    char *cursor;
} scanner_t;

int scanner_next(scanner_t *s, token_t *t);

#endif
