#include <mycalc.h>

FILE *source, *object;

token_t lookahead;

char lexeme[MAX_ID_LEN];

main(int argc, char *argv[])
{
	while(1) {
		E_lvl = 0;
		T_lvl = 0;
		F_lvl = 0;
		fflush(stdin);
		fprintf(stdout,"> ");
		
		source = stdin;        
        	object = stdout;	
		/* get the first token to begin the parsing */
		lookahead = gettoken(source);	

		/* call the grammar initial symbol */
		E_lvl++;
		expr();			
		//if(match(10))
		E_lvl--;
		fprintf(stdout,"\n");
		fprintf(stdout, "E_lvl: %d\n", E_lvl);
		fprintf(stdout, "T_lvl: %d\n", T_lvl);
		fprintf(stdout, "F_lvl: %d\n", F_lvl);
		if(!E_lvl && !T_lvl && !F_lvl) {
			fprintf(stdout, "Stack OK.\n");
		} else {
			fprintf(stdout, "Invalid sentence.\n");
		}
		fprintf(stdout,"\n");	
	}
	return 0;
}
