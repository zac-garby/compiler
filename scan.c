#include "scan.h"

static int scanner_is_eof(scanner_t *s);
static char scanner_advance(scanner_t *s);
static char scanner_peek(scanner_t *s);
static int is_digit(char ch);

int scanner_next(scanner_t *s, token_t *t) {
    char ch;
    int i, done;
    
    while (*s->cursor == ' ' || *s->cursor == '\t' || *s->cursor == '\n') {
        scanner_advance(s);
    }
    
    ch = *s->cursor;
    t->lexeme[0] = ch;
    
    if (ch == '\0') {
        t->type = T_EOF;
    } else if (ch == '+') {
        t->type = T_PLUS;
    } else if (ch == '-') {
        t->type = T_MINUS;
    } else if (ch == '(') {
        t->type = T_LPAREN;
    } else if (ch == ')') {
        t->type = T_RPAREN;
    } else if (is_digit(ch)) {
        t->type = T_INT;
        
        done = 0;
        for (i = 1; !done; i++) {
            if (is_digit(scanner_peek(s))) {
                ch = scanner_advance(s);
                t->lexeme[i] = ch;
            } else {
                done = 1;
            }
        }
    } else {
        return 0;
    }
    
    scanner_advance(s);
    
    return 1;
}

static int scanner_is_eof(scanner_t *s) {
    return *s->cursor == '\0';
}

static char scanner_advance(scanner_t *s) {
    if (!scanner_is_eof(s)) {
        s->cursor++;
    }
    
    return *s->cursor;
}

static char scanner_peek(scanner_t *s) {
    return s->cursor[1];
}

int is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}
