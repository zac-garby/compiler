#include "scan.h"

static int scanner_is_eof(scanner_t *s);
static char scanner_advance(scanner_t *s);
/* static char scanner_peek(scanner_t *s); */
static int is_digit(char ch);
static int is_letter(char ch);
static int is_letter_or_digit(char ch);
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
        scanner_advance(s);
    } else if (ch == '+') {
        t->type = T_PLUS;
        scanner_advance(s);
    } else if (ch == '-') {
        t->type = T_MINUS;
        scanner_advance(s);
    } else if (ch == '*') {
        t->type = T_TIMES;
        scanner_advance(s);
    } else if (ch == '/') {
        t->type = T_DIVIDE;
        scanner_advance(s);
    } else if (ch == '(') {
        t->type = T_LPAREN;
        scanner_advance(s);
    } else if (ch == ')') {
        t->type = T_RPAREN;
        scanner_advance(s);
    } else if (ch == '=') {
        t->type = T_ASSIGN;
        scanner_advance(s);
    } else if (ch == ';') {
        t->type = T_SEMI;
        scanner_advance(s);
    } else if (ch == ',') {
        t->type = T_COMMA;
        scanner_advance(s);
    } else if (scan_while(s, t->lexeme, is_digit)) {
        t->type = T_INT;
    } else if (is_letter(ch)) {
        t->type = T_IDENT;
        scan_while(s, t->lexeme, is_letter_or_digit);
    } else {
        return 0;
    }
    
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

    if (found) {
        lexeme[i] = '\0';
    }
    
    return found;
}

char *tok_type(toktype_t type) {
    switch (type) {
    case T_EOF:
        return "EOF";
    case T_INT:
        return "INT";
    case T_PLUS:
        return "PLUS";
    case T_MINUS:
        return "MINUS";
    case T_TIMES:
        return "TIMES";
    case T_DIVIDE:
        return "DIVIDE";
    case T_LPAREN:
        return "LPAREN";
    case T_RPAREN:
        return "RPAREN";
    case T_IDENT:
        return "IDENT";
    case T_ASSIGN:
        return "ASSIGN";
    case T_SEMI:
        return "SEMI";
    case T_COMMA:
        return "COMMA";
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

static int is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

static int is_letter(char ch) {
    return ('a' <= ch && ch <= 'z')
        || ('A' <= ch && ch <= 'Z')
        || (ch == '_');
}

static int is_letter_or_digit(char ch) {
    return is_digit(ch) || is_letter(ch);
}
