#include <stdio.h>

#include <parser.h>
#include <args.h>

#define PROMPT ">"

int exit_with_error(int);

extern token_t gettoken(FILE *);

extern double expr(void);

