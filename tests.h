#include <stdio.h>

#include <parser.h>

int T_FLAG;

extern FILE *object;

extern FILE *source;

extern token_t lookahead;

extern token_t gettoken(FILE *);

extern int expr(void);

int test(const char*, int);

int do_tests(void);