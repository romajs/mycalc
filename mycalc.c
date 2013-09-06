#include <mycalc.h>

FILE *source, *object;

token_t lookahead;

char lexeme[MAX_ID_LEN];

main(int argc, char *argv[])
{
	source = stdin;
        
        object = stdout;

	/* get the first token to begin the parsing */
	lookahead = gettoken(source);

	/* call the grammar initial symbol */
	expr();
        
        fprintf(stdout,"\n");

	return 0;
}
