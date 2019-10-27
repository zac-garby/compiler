#include <stdio.h>
#include <stdlib.h>

#include "scan.h"
#include "ast.h"
#include "parser.h"

void process_line(char *line);

int main(int argc, char **argv) {
    char *line, *new_line, ch;
    int line_cap, i;
    
    line_cap = 32;
    line = malloc(line_cap);
    
    while (1) {
        printf("> ");
        
        i = 0;
        while ((ch = getchar()) != '\n') {
            if (i + 1 >= line_cap) {
                line_cap += 32;
                new_line = realloc(line, line_cap);
    
                if (new_line == NULL) {
                    free(line);
                    printf("could not allocate enough memory for input line\n");
                    exit(1);
                } else {
                    line = new_line;
                }
            }
                       
            line[i++] = ch;
        }
        
        line[i] = '\0';
        
        process_line(line);
    }
    
    free(line);
    free(new_line);
    
    return 0;
}

void process_line(char *line) {
    scanner_t s;
    parser_t p;
    expr_t e;
    
    s.cursor = &(line[0]);
    p.s = &s;
    p.cur.lexeme = malloc(128 * sizeof(char));
    parser_advance(&p);

    if (parse_expr(&p, &e)) {
        print_expr(0, &e);
    } else {
        printf("%s\n", p.err.message);
    }

    free(p.cur.lexeme);
}
