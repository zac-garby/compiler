#include <stdio.h>

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
    }
}

void print_stmt(int indent, stmt_t *s) {
    int i, j;

    for (i = 0; i < indent; i++) printf("| ");

    switch (s->type) {
    case ST_COMPOUND:
        printf("COMPOUND:\n");
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