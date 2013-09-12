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
	while(1) {
		
		source = stdin;        
        object = stdout;	
		
		if(!(child = fork())) {
			//fprintf(object, "child: %d\n", child);			
			fprintf(stdout,"> ");
			
			/* get the first token to begin the parsing */
			lookahead = gettoken(source);
			
			/* call the grammar initial symbol */		
			fprintf(object, " = %d\n", expr());
			exit(OK);
		} else if (child == -1) {
			perror("fork error.");
			exit(EXIT_FAILURE);
		} else {			
			//fprintf(stdout,"waiting parser...\n");
			waitpid(child, &status, WUNTRACED);			
			fprintf(object, "parser exited with status: %d\n", status);	
		}
		
		fetch_status(status);
		fprintf(object, "E_lvl: %d\n\n", E_lvl);		
		fflush(source);
	}
	return EXIT_SUCCESS;
}
