#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

void print_expr(int indent, expr_t *e) {
    int i;
    
    for (i = 0; i < indent; i++) printf("| ");

    switch (e->type) {
    case EX_INT:
        printf("INT: %d\n", e->integer);
        break;
    case EX_IDENT:
        printf("IDENT: %s\n", e->ident);
        break;
    case EX_INFIX:
        printf("INFIX: %s\n", tok_type(e->infix->op));
        print_expr(indent+1, e->infix->left);
        print_expr(indent+1, e->infix->right);
        break;
    case EX_PREFIX:
        printf("PREFIX: %s\n", tok_type(e->prefix->op));
        print_expr(indent+1, e->prefix->e);
        break;
    }
}

void print_stmt(int indent, stmt_t *s) {
    int i, j;

    for (i = 0; i < indent; i++) printf("| ");

    switch (s->type) {
    case ST_COMPOUND:
        printf("COMPOUND (%d):\n", s->compound->amount);
        for (j = 0; j < s->compound->amount; j++) {
            print_stmt(indent+1, &s->compound->statements[j]);
        }
        break;
    case ST_EXPR:
        printf("EXPR:\n");
        print_expr(indent+1, s->expr);
        break;
    }
}

void free_expr(expr_t *e) {
    switch (e->type) {
    case EX_INT:
        break;
    case EX_IDENT:
        free(e->ident);
        break;
    case EX_INFIX:
        free_expr(e->infix->left);
        free_expr(e->infix->right);
        free(e->infix);
        break;
    case EX_PREFIX:
        free_expr(e->prefix->e);
        free(e->prefix);
        break;
    }

    if (e->mallocd) {
        free(e);
    }
}

void free_stmt(stmt_t *s) {
    int i;
    
    switch (s->type) {
    case ST_COMPOUND:
        for (i = 0; i < s->compound->amount; i++) {
            free_stmt(&s->compound->statements[i]);
        }
        free(s->compound->statements);
        break;
    case ST_EXPR:
        free_expr(s->expr);
        break;
    }

    if (s->mallocd) {
        free(s);
    }
}
