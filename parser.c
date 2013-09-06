#include <parser.h>


/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 * expr -> term { [+|-] term }
 *
 *
 *
 *             -------(+|-)-------
 *             |                 |
 *             v                 |
 * (expr)--->(init)--->(term)---(?)-->(end)
 *
 */
void expr(void)
{
	if(lookahead=='-'){
		match('-');
	}
init:
	term();
	if(lookahead == '+' || lookahead == '-') {
		match(lookahead);
		goto init;
	}
}
/*
 * term -> fact { [*|/] fact }
 *
 *             -------(*|/)-------
 *             |                 |
 *             v                 |
 * (term)--->(init)--->(fact)---(?)-->(end)
 *
 */
void term(void)
{
init:
	fact();
	if(lookahead == '*' || lookahead == '/') {
		match(lookahead);
		goto init;
	}
}
/*
 * fact -> ID
 *       | NUM
 *       | ( expr )
 *
 */
void fact(void)
{
	switch(lookahead){
	case ID: case NUM:
		match(lookahead);
		break;
	default:
		match('('); expr(); match(')');
	}
}

