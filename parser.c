#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
        return 0;
    }

    if (p->cur.type == T_ASSIGN) {
        expr_t *infix;
        expr_infix_t *inf;
        
        parser_advance(p);

        infix = malloc(sizeof(expr_t));
        infix->mallocd = 1;
        
        inf = malloc(sizeof(expr_infix_t));

        infix->type = EX_INFIX;
        infix->infix = inf;

        inf->op = T_ASSIGN;
        
        inf->left = malloc(sizeof(expr_t));
        inf->left->mallocd = 1;
        
        inf->right = malloc(sizeof(expr_t));
        inf->right->mallocd = 1;

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
        return 0;
    }

    while (p->cur.type == T_PLUS || p->cur.type == T_MINUS) {
        expr_t *infix;
        expr_infix_t *inf;
        
        toktype_t op = p->cur.type;
        parser_advance(p);

        infix = malloc(sizeof(expr_t));
        infix->mallocd = 1;
        
        inf = malloc(sizeof(expr_infix_t));

        infix->type = EX_INFIX;
        infix->infix = inf;
        
        inf->op = op;
        
        inf->left = malloc(sizeof(expr_t));
        inf->left->mallocd = 1;
        
        inf->right = malloc(sizeof(expr_t));
        inf->right->mallocd = 1;
        
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
    if (!parse_call(p, e)) {
        return 0;
    }

    while (p->cur.type == T_TIMES || p->cur.type == T_DIVIDE) {
        expr_t *infix;
        expr_infix_t *inf;
        
        toktype_t op = p->cur.type;
        parser_advance(p);

        infix = malloc(sizeof(expr_t));
        infix->mallocd = 1;
        
        inf = malloc(sizeof(expr_infix_t));

        infix->type = EX_INFIX;
        infix->infix = inf;
        
        inf->op = op;
        
        inf->left = malloc(sizeof(expr_t));
        inf->left->mallocd = 1;
        
        inf->right = malloc(sizeof(expr_t));
        inf->right->mallocd = 1;
        
        memcpy(inf->left, e, sizeof(expr_t));

        if (!parse_call(p, inf->right)) {
            set_error(p, "expected an expression following an operator");
            return 0;
        }

        memcpy(e, infix, sizeof(expr_t));
    }

    return 1;
}

int parse_call(parser_t *p, expr_t *e) {
    if (!parse_atom(p, e)) {
	return 0;
    }

    if (p->cur.type == T_LPAREN) {
	expr_call_t *call;
	expr_t *new_args;
	int capacity = 2;

	call = malloc(sizeof(expr_call_t));
	
	call->fn = malloc(sizeof(expr_t));
	memcpy(call->fn, e, sizeof(expr_t));
	call->fn->mallocd = 1;
	
	call->args = malloc(capacity * sizeof(expr_t));
	call->n_args = 0;
	
	parser_advance(p);

	while (p->cur.type != T_EOF && p->cur.type != T_RPAREN) {
	    if (call->n_args >= capacity) {
		capacity += 4;
		new_args = realloc(call->args, capacity * sizeof(expr_t));

		if (new_args == NULL) {
		    printf("could not allocate enough memory to hold the function arguments\n");
		    exit(1);
		} else {
		    call->args = new_args;
		}
	    }
	    
	    int ok = parse_expr(p, &call->args[call->n_args]);
	    if (!ok) {
		return 0;
	    }

	    call->n_args++;

	    if (p->cur.type == T_COMMA) {
		parser_advance(p);
	    } else if (p->cur.type == T_RPAREN) {
		break;
	    } else {
		set_error(p, "missing comma in function arguments");
		return 0;
	    }
	}

	if (p->cur.type == T_RPAREN) {
	    parser_advance(p);
	} else {
	    set_error(p, "missing closing bracket in function arguments");
	    return 0;
	}
	
	e->type = EX_CALL;
	e->call = call;
    }

    return 1;
}

int parse_atom(parser_t *p, expr_t *e) {
    if (parse_integer(p, e)) {
        return 1;
    }

    if (parse_ident(p, e)) {
        return 1;
    }
    
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

    if (p->cur.type == T_PLUS || p->cur.type == T_MINUS) {
        expr_prefix_t *pref = malloc(sizeof(expr_prefix_t));

        pref->op = p->cur.type;
        pref->e = malloc(sizeof(expr_t));
        pref->e->mallocd = 1;
        
        parser_advance(p);
        if (!parse_expr(p, pref->e)) {
            return 0;
        }

        e->type = EX_PREFIX;
        e->prefix = pref;

        return 1;
    }

    set_error(p, "no atomic expression to parse. looking for parenthesised expression, unary operator, integer literal, or identifier.");

    return 0;
}

int parse_stmt(parser_t *p, stmt_t *s) {
    s->type = ST_EXPR;
    
    s->expr = malloc(sizeof(expr_t));
    s->expr->mallocd = 1;
    
    if (!parse_expr( p, s->expr)) {
        // set_error(p, "expected a statement");
        return 0;
    }

    if (p->cur.type != T_SEMI) {
        set_error(p, "semicolon missing after statement");
        return 0;
    }

    parser_advance(p);

    return 1;
}

int parse_compound(parser_t *p, stmt_t *s) {
    int i, capacity;
    stmt_t *new_arr;
    
    s->type = ST_COMPOUND;
    s->compound = malloc(sizeof(stmt_compound_t));

    capacity = 4;
    s->compound->amount = 0;
    s->compound->statements = malloc(sizeof(stmt_t) * 4);
    
    for (i = 0; ; i++) {
        if (i >= capacity) {
            capacity += 16;
            new_arr = realloc(s->compound->statements, capacity);
            
            if (new_arr == NULL) {
                free(s->compound->statements);
                printf("ran out of memory to allocate array storage\n");
                exit(1);
            } else {
                s->compound->statements = new_arr;
            }
        }

	if (p->cur.type == T_EOF) {
	    return 1;
	}
	
        if (parse_stmt(p, &s->compound->statements[i])) {
            s->compound->amount++;
        } else {
            if (p->err.message == NULL) {
                break;
            } else {
                return 0;
            }
        }
    }

    return 1;
}
