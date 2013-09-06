#include <stdio.h>
#include <mytype.h>
#include <tokens.h>

/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 * expr -> [-] term { [+|-] term }
 *
 *
 *
 *                    --(+|-)---
 *        --(-)--     |        |
 *        |     |     |        |
 *        |     v     v        |
 * (expr)---------->(term)--->(?)-->(@)
 *
 */
void expr(void);
/*
 * term -> fact { [*|/] fact }
 *
 *             -------(*|/)-------
 *             |                 |
 *             v                 |
 * (term)--->(init)--->(fact)---(?)-->(end)
 *
 */
void term(void);
/*
 * fact -> ID
 *       | NUM
 *       | ( expr )
 *
 */
void fact(void);

extern void match(token_t);

extern FILE *object;

extern token_t lookahead;

extern char lexeme[];
