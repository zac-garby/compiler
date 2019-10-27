#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "scan.h"

static void set_error(parser_t *p, char *msg);

static void set_error(parser_t *p, char *msg) {
    parse_error_t err;
    err.message = msg;
    p->err = err;
}

int parser_advance(parser_t *p) {
    int ok = scanner_next(p->s, &p->cur);
    
    if (!ok) {
        set_error(p, "error during scanning");
    }

    return ok;
}

int parse_expr(parser_t *p, expr_t *e) {
    return parse_assignment(p, e);
}

int parse_integer(parser_t *p, expr_t *e) {
    if (p->cur.type != T_INT) {
        set_error(p, "expected an integer literal");
        return 0;
    }

    e->type = EX_INT;
    e->integer = atoi(p->cur.lexeme);

    parser_advance(p);

    return 1;
}

int parse_ident(parser_t *p, expr_t *e) {
    char *ident;
    size_t length;
    
    if (p->cur.type != T_IDENT) {
        set_error(p, "expected an identifier");
        return 0;
    }

    e->type = EX_IDENT;

    length = strlen(p->cur.lexeme);
    ident = malloc(length * sizeof(char));
    strcpy(ident, p->cur.lexeme);
    e->ident = ident;

    parser_advance(p);

    return 1;
}

int parse_assignment(parser_t *p, expr_t *e) {
    if (!parse_term(p, e)) {
        set_error(p, "expected an expression");
        return 0;
    }

    if (p->cur.type == T_ASSIGN) {
        expr_t *infix;
        expr_infix_t *inf;
        
        parser_advance(p);

        infix = malloc(sizeof(expr_t));
        inf = malloc(sizeof(expr_infix_t));

        infix->type = EX_INFIX;
        infix->infix = inf;

        inf->op = T_ASSIGN;
        inf->left = malloc(sizeof(expr_t));
        inf->right = malloc(sizeof(expr_t));

        memcpy(inf->left, e, sizeof(expr_t));

        if (!parse_term(p, inf->right)) {
            set_error(p, "expected an expression following an assignment operator");
            return 0;
        }

        memcpy(e, infix, sizeof(expr_t));
    }

    return 1;
}

int parse_term(parser_t *p, expr_t *e) {
    if (!parse_factor(p, e)) {
        set_error(p, "expected an expression");
        return 0;
    }

    while (p->cur.type == T_PLUS || p->cur.type == T_MINUS) {
        expr_t *infix;
        expr_infix_t *inf;
        
        toktype_t op = p->cur.type;
        parser_advance(p);

        infix = malloc(sizeof(expr_t));
        inf = malloc(sizeof(expr_infix_t));

        infix->type = EX_INFIX;
        infix->infix = inf;
        
        inf->op = op;
        inf->left = malloc(sizeof(expr_t));
        inf->right = malloc(sizeof(expr_t));
        
        memcpy(inf->left, e, sizeof(expr_t));

        if (!parse_factor(p, inf->right)) {
            set_error(p, "expected an expression following an operator");
            return 0;
        }
        
        memcpy(e, infix, sizeof(expr_t));
    }

    return 1;
}

int parse_factor(parser_t *p, expr_t *e) {
    if (!parse_atom(p, e)) {
        set_error(p, "expected an expression");
        return 0;
    }

    while (p->cur.type == T_TIMES || p->cur.type == T_DIVIDE) {
        expr_t *infix;
        expr_infix_t *inf;
        
        toktype_t op = p->cur.type;
        parser_advance(p);

        infix = malloc(sizeof(expr_t));
        inf = malloc(sizeof(expr_infix_t));

        infix->type = EX_INFIX;
        infix->infix = inf;
        
        inf->op = op;
        inf->left = malloc(sizeof(expr_t));
        inf->right = malloc(sizeof(expr_t));
        
        memcpy(inf->left, e, sizeof(expr_t));

        if (!parse_atom(p, inf->right)) {
            set_error(p, "expected an expression following an operator");
            return 0;
        }
        
        memcpy(e, infix, sizeof(expr_t));
    }

    return 1;
}

int parse_atom(parser_t *p, expr_t *e) {
    if (p->cur.type == T_LPAREN) {
        parser_advance(p);

        if (!parse_expr(p, e)) {
            return 0;
        }

        if (p->cur.type != T_RPAREN) {
            set_error(p, "missing closing parenthesis");
            return 0;
        }

        parser_advance(p);

        return 1;
    }

    if (parse_integer(p, e)) {
        return 1;
    }

    if (parse_ident(p, e)) {
        return 1;
    }

    return 0;
}
