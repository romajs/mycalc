#include <mycalc.h>

FILE *source, *object, *debug;

token_t lookahead;

char lexeme[MAX_ID_LEN];

#define PROMPT "mycalc>"

mycalc(void) {

	pid_t child;
	int status = -1;
	int result;
    
	if(!(child = fork())) { // inicia novo processo
		//fprintf(object, "child: %d\n", child);		
		
		/* get the first token to begin the parsing */
		lookahead = gettoken(source);
		
		/* call the grammar initial symbol */
		result = expr();		
		fprintf(object, " = %d\n", result);
		exit(OK);
	} else if (child == -1) {
		fprintf(object, "fork error.");
		exit(EXIT_FAILURE);
	} else {			
		//fprintf(stdout,"waiting parser...\n");
		waitpid(child, &status, WUNTRACED);	// aguarda o filho terminar	
	}
	
	print_status_message(status);
	fflush(source);
	return result;
}

print_status_message(int status) {		
	fprintf(object, "status: %d\n", WEXITSTATUS(status));
	switch(WEXITSTATUS(status)) {
	case OK:
		fprintf(object, "OK.");
		break;
	case TOKEN_MISMATCH:		
		fprintf(object, "Token mismatch (%d).", lookahead);
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
		if(strcmp(argv[i], "-t") == 0) { // executa testes
      do_tests();
    }
  }
}

test(const char* expression, int expected) {
	fprintf(source, "%s\n", expression);
	if(mycalc() == expected) {
		fprintf(object, "OK.\n");
	} else {
		fprintf(object, "Failed.\n");
	}
}

do_tests(void) {
	fprintf(object, "Initiating tests...\n");	
	source = object;
	// obs: ainda não funcional
	test("1+2", 3);
	//test("1+2+3*4", 15);
	//test("1+2*3+4", 11);
	//test("1+2*3*4", 25);
	//test("1*2+3+4", 9);
	//test("1*2+3*4", 25);
	//test("1*2*3+4", 20);
	//test("1*2*3*4", 24) ;
	//test("",);
	source = stdin;
}

main(int argc, char *argv[])
{
	source = stdin;        
  object = stdout;
  
  validate_arguments(argc, argv);
    
	while(1) {				
		fprintf(object, "%s", PROMPT);
		mycalc();		
		fprintf(object, "\n");		
	}
	return EXIT_SUCCESS;
}
