#include <stdio.h>

#include <parser.h>
#include <args.h>

#define PROMPT "mycalc>"

extern token_t gettoken(FILE *);

extern int expr(void);