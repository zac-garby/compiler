#include "scan.h"

static int scanner_is_eof(scanner_t *s);
static char scanner_advance(scanner_t *s);
static char scanner_peek(scanner_t *s);
static int is_digit(char ch);
static int scan_while(scanner_t *s, char *lexeme, int(*pred)(char s));

int scanner_next(scanner_t *s, token_t *t) {
    char ch;
    
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
    } else if (scan_while(s, t->lexeme, is_digit)) {
        t->type = T_INT;
    } else {
        return 0;
    }
    
    scanner_advance(s);
    
    return 1;
}

static int scan_while(scanner_t *s, char *lexeme, int(*f)(char s)) {
    int i, found;
    found = i = 0;
    
    while (f(*s->cursor)) {
        found = 1;
        lexeme[i++] = *s->cursor;
        scanner_advance(s);
    }
    
    return found;
}

char *tok_type(int type) {
    switch (type) {
    case T_EOF:
        return "EOF";
    case T_INT:
        return "INT";
    case T_PLUS:
        return "PLUS";
    case T_MINUS:
        return "MINUS";
    case T_LPAREN:
        return "LPAREN";
    case T_RPAREN:
        return "RPAREN";
    default:
        return "UNDEFINED";
    }
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

/*
static char scanner_peek(scanner_t *s) {
    return s->cursor[1];
}
*/

int is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}
