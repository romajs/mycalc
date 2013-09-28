#include <stdio.h>

#include <parser.h>
#include <args.h>

#define PROMPT ">"

extern int E_lvl;	

extern token_t lookahead;

extern char lexeme[];

extern void debug(const char *, ...);

extern token_t gettoken(FILE *);

extern double expr(void);

int mycalc(double*); 

void print_status_message(int);

void exit_with_error(int);