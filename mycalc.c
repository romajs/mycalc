#include <mycalc.h>

FILE *source;

token_t lookahead;

main(int argc, char *argv[])
{
	source = stdin;

	/* get the first token to begin the parsing */
	lookahead = gettoken(source);

	/* call the grammar initial symbol */
	expr();

	return 0;
}
