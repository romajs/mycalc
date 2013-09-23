#include <mycalc.h>

FILE *source, *object, *debug;

token_t lookahead;

char lexeme[MAX_ID_LEN];

print_status_message(int status) {		
	fprintf(debug, "returned status: %d\n", status);
	switch(status) {
	case OK:
		fprintf(object, "OK.");
		break;
	case TOKEN_MISMATCH:		
		fprintf(object, "Token mismatch \"%d\".", lookahead);
		break;
  case ID_NOT_DECLARED:
    fprintf(object, "Identifier not declared. \"%s\".", lexeme);
    break;
	default:
		fprintf(object, "Uncaught status \"%d\".", status);
		break;
	}
	fprintf(object, "\n");
}

main(int argc, char *argv[])
{
   source = stdin;        
   object = stdout;
   
   match_args(argc, argv);
    
	while(1) {	  
      lookahead = -1;
      lexeme[0] = 0;
      //fflush(source);
      
      //if(!(child = fork())) { // inicia novo processo
         //fprintf(object, "child: %d\n", child);		
         
         fprintf(object, "%s", PROMPT);    	  
      
         /* get the first token to begin the parsing */
         lookahead = gettoken(source);
         
         /* call the grammar initial symbol */				 
         fprintf(object, " = %d\n", expr());
     
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
  print_status_message(status);
}
