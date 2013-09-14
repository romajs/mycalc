#include <mycalc.h>

FILE *source, *object;

token_t lookahead;

char lexeme[MAX_ID_LEN];

print_status_message(int status) {		
	fprintf(object, "status: %d\n", WEXITSTATUS(status));
	switch(WEXITSTATUS(status)) {
	case OK:
		fprintf(object, "OK.");
		break;
	case TOKEN_MISMATCH:		
		fprintf(object, "Token mismatch.");
		break;
	default:
		fprintf(object, "Uncaught status.");
		break;
	}
	fprintf(object, "\n");
}

main(int argc, char *argv[])
{
	int status;
	pid_t child;
	while(1) {
		
		status = -1;
		source = stdin;        
        object = stdout;	
		
		if(!(child = fork())) { // inicia novo processo
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
			waitpid(child, &status, WUNTRACED);	// aguarda o filho terminar	
		}
		
		print_status_message(status);
		//fprintf(object, "E_lvl: %d\n", E_lvl);
		fprintf(object, "\n");		
		fflush(source);
	}
	return EXIT_SUCCESS;
}
