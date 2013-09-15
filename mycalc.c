#include <mycalc.h>

FILE *source, *object, *debug;

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

void validate_arguments(int argc, char *argv[]) {
  int i; // manipulação de parâmetros
  //fprintf(object, "argc: %i\n", argc);
  for(i = 1; i < argc; i++) {
    //fprintf(object, "argv[%i]: %s\n", i, argv[i]);
    if(strcmp(argv[i], "-d") == 0) { // habilita  modo debug
      debug = object;
    }
  }
}

main(int argc, char *argv[])
{
	int status;
	pid_t child;
 
	source = stdin;        
  object = stdout;
  
  validate_arguments(argc, argv);
    
	while(1) {
		
		status = -1;	
    
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
		fprintf(object, "\n");		
		fflush(source);
	}
	return EXIT_SUCCESS;
}
