#include <mycalc.h>

FILE *source, *object, *debug;

token_t lookahead;

char lexeme[MAX_ID_LEN];

int error = 0;

print_status_message(int status) {		
	fprintf(debug, "returned status: %d\n", status);
	switch(status) {
	case OK:
		fprintf(object, "OK.\n");
		break;
	case TOKEN_MISMATCH:		
		fprintf(object, "Token mismatch.\n");
		break;
  case ID_NOT_DECLARED:
    fprintf(object, "ID not declared.\n");
    break;
  case STACK_MEM_OVERFLOW:
    fprintf(object, "Stack memory overflow.\n");
    break;
	default:
		fprintf(object, "Uncaught status \"%d\".\n", status);
		break;
	}
  fprintf(object, "Lexeme = \"%s\", lookahead = \"%d\".\n", lexeme, lookahead);
}

main(int argc, char *argv[])
{  
  source = stdin;        
  object = stdout;

  match_args(argc, argv);
    
	while(1) {	
    fflush(source);
    error = 0;
    lookahead = -1;
    lexeme[0] = 0;
    //if(!(child = fork())) { // inicia novo processo
       //fprintf(object, "child: %d\n", child);		
       
       fprintf(object, "%s", PROMPT);    	  
    
       /* get the first token to begin the parsing */
       lookahead = gettoken(source);

       /* skips blank feed line */ 
       if(lookahead == EOF) {
         fprintf(object, "\n");
         continue;
       }
       
       /* call the grammar initial symbol */
       double value = expr();
       if(!error) {
        fprintf(object, " = %.2f\n", value);
       }
   
    /*   exit(OK);
    } else if (child == -1) {
       fprintf(object, "fork error.\n");
       exit(EXIT_FAILURE);
    } else {			
       //fprintf(object, "waiting for child \"%d\"...\n", child);
       waitpid(child, &status, WUNTRACED);	// aguarda o filho terminar	
    }
    print_status_message(status);*/	
    fprintf(object, "\n");	
	}
	return EXIT_SUCCESS;
}

exit_with_error(int status) {  
  error = 1;
  print_status_message(status);
}
