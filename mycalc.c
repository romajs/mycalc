#include <mycalc.h>

FILE *source, *object;

token_t lookahead;

char lexeme[MAX_ID_LEN];

main(int argc, char *argv[])
{
	while(1) {
	   E_lvl = 0;
		fflush(stdin);
		fprintf(stdout,"> ");
		
		source = stdin;        
        	object = stdout;	
		/* get the first token to begin the parsing */
		lookahead = gettoken(source);	

		/* call the grammar initial symbol */
		expr();			
		fprintf(stdout,"E_lvl: %d\n\n", E_lvl);	
	}
	return 0;
}
