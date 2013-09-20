#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <mytype.h>
#include <tokens.h>

extern FILE *source;
extern FILE *debug;
extern token_t lookahead;
extern char lexeme[];