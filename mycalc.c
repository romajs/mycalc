#include <mycalc.h>

FILE *source, *object;

token_t lookahead;

char lexeme[MAX_ID_LEN];

fetch_status(int status) {
	switch(status) {
	case OK:
		fprintf(object, "OK.\n");
		break;
	case 256: // deixei assim porque o 'status' retorna meio estranho
	//case TOKEN_MISMATCH:		
		fprintf(object, "Token mismatch.\n");
		break;
	default:
		break;
	}
}

main(int argc, char *argv[])
{
	int status;
	pid_t child;
	fprintf(stdout, "exit: %d\n",  TOKEN_MISMATCH);
	while(1) {
	
	    E_lvl = 0;
		fprintf(stdout,"> ");
		
		source = stdin;        
        object = stdout;	
		
		if(!(child = fork())) {
			//fprintf(object, "child: %d\n", child);
			
			/* get the first token to begin the parsing */
			lookahead = gettoken(source);
			
			/* call the grammar initial symbol */
			expr();			
			
			exit(OK);
		} else if (child == -1) {
			perror("fork error.");
			exit(EXIT_FAILURE);
		} else {			
			//fprintf(stdout,"waiting...\n");
			waitpid(child, &status, WUNTRACED);			
			fprintf(object, "exit: %d\n", (int)status);	
		}
		
		fetch_status(status);
		fprintf(object, "E_lvl: %d\n\n", E_lvl);		
		fflush(source);
	}
	return EXIT_SUCCESS;
}
