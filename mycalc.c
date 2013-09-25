#include <mycalc.h>

FILE *source, *object, *debug;

int error = 0;

main(int argc, char *argv[])
{  
  source = stdin;        
  object = stdout;

  match_args(argc, argv);
    
	while(1) {	
		fprintf(object, "%s", PROMPT);  
		double value;				
		if(mycalc(&value)) {
			fprintf(object, " = %.2f\n", value);
		}
	}
	return EXIT_SUCCESS;
}

mycalc(double *value) {  	  
  
	/* cleaning everything before start */
	error = 0;	
	lookahead = -1;
	lexeme[0] = 0; 
	fflush(source); // não adianta muito, mas...
	
	/* get the first token to begin the parsing */
	lookahead = gettoken(source);

	/* skips blank feed line */ 
	if(lookahead == EOF) goto MYCALC_EXIT;

	/* call the grammar initial symbol */
	*value = expr();
MYCALC_EXIT:
	//fprintf(object, "\n");	
	return !error;
}

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

exit_with_error(int status) {  
  error = 1;
  print_status_message(status);
}