#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

void print_expr(int indent, expr_t *e) {
    int i, j;
    
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
    case EX_CALL:
	printf("CALL (%d args)\n", e->call->n_args);

	for (i = 0; i < indent+1; i++) printf("| ");
	printf("FUNCTION:\n");
	print_expr(indent+2, e->call->fn);

	for (i = 0; i < indent+1; i++) printf("| ");
	printf("ARGUMENTS (%d):\n", e->call->n_args);
	for (i = 0; i < e->call->n_args; i++) {
	    for (j = 0; j < indent+2; j++) printf("| ");
	    
	    printf("ARG #%d\n", i);
	    print_expr(indent+3, &e->call->args[i]);
	}
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
    int i;
    
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
    case EX_CALL:
	free_expr(e->call->fn);

	for (i = 0; i < e->call->n_args; i++) {
	    free_expr(&e->call->args[i]);
	}

	free(e->call);
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
