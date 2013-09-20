#include <stdio.h>

#include <parser.h>
#include <args.h>

#define PROMPT ">"

extern token_t gettoken(FILE *);

extern int expr(void);