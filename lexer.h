#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <mytype.h>
#include <tokens.h>

extern FILE *source;

extern FILE *object;

extern void debug(const char *, ...);

extern token_t lookahead;

extern char lexeme[];

extern exit_with_error(int);