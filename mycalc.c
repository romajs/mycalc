#include <mycalc.h>

FILE *source, *object;

token_t lookahead;

char lexeme[MAX_ID_LEN];

main(int argc, char *argv[])
{
	while(1) {
		fflush(stdin);
		fprintf(stdout,"> ");
		
		source = stdin;        
        object = stdout;	
		/* get the first token to begin the parsing */
		lookahead = gettoken(source);	

		/* call the grammar initial symbol */
		expr();		
		match(10);
		fprintf(stdout,"\n\n");	
	}
	return 0;
}
