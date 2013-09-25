#include <stdio.h>

#include <parser.h>
#include <args.h>

#define PROMPT ">"

extern token_t lookahead;
extern char lexeme[];
extern token_t gettoken(FILE *);
extern double expr(void);