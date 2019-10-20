#include <stdio.h>
#include <stdlib.h>

#include "scan.h"

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
    token_t t;
    scanner_t s;
    int i;
    
    t.lexeme = malloc(128);
    s.cursor = &(line[0]);
    
    for (scanner_next(&s, &t); t.type != T_EOF; scanner_next(&s, &t)) {
        printf("%-8s %s\n", tok_type(t.type), t.lexeme);
        
        /* clear string to null characters */
        for (i = 0; t.lexeme[i] != '\0'; i++) {
            t.lexeme[i] = '\0';
        }
    }
    
    free(t.lexeme);
}
