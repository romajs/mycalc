#include <mycalc.h>

FILE *source, *object, *debug;

int error; // flag de erro

main(int argc, char *argv[]) {

  // define padrões de entrada e saída (output/input)
  source = stdin;        
  object = stdout;

  // consome os argumentos de mycalc (se existirem)
  match_args(argc, argv); 
    
  // loop principal de execução (interpretador)  
	while(1) {	
		fprintf(object, "%s", PROMPT);  
		double value;				
		if(mycalc(&value)) { // se não houver erro, exibe o valor
			fprintf(object, " = %.2f\n", value);
		}
	}
	return EXIT_SUCCESS;
}

// função principal do programa, retorno:
// 1 - SUCESSO
// 0 - FALHOU
mycalc(double *value) {  	  
  	
	// set no errors	
	error = 0;	
	
	// clear parser before start 
	lookahead = EOF;
	lexeme[0] = 0;
	fflush(source); // não adianta muito, mas...
	
	// get the first token to begin the parsing
	lookahead = gettoken(source);

	// skips blank feed line
	if(lookahead == EOF) goto MYCALC_EXIT;

	// call the grammar initial symbol
	*value = expr();
MYCALC_EXIT:
	//fprintf(object, "\n");	
	return !error;
}

// função que exibe a mensagem referente ao status retornado
void print_status_message(int status) {		
	fprintf(debug, "returned status: %d\n", status);
	switch(status) {
	case OK:
		fprintf(object, "OK.\n");
		break;
	case TOKEN_MISMATCH:		
		fprintf(object, "Token mismatch.\n");
		break;
  case ID_NOT_DECLARED:
    fprintf(object, "Undeclared ID.\n");
    break;
  case STACK_MEM_OVERFLOW:
    fprintf(object, "Stack memory overflow.\n");
    break;
	case MAX_DIG_OVERFLOW:
    fprintf(object, "Maximum numeric digits exceeded.\n");
    break;
	default:
		fprintf(object, "Uncaught status \"%d\".\n", status);
		break;
	}
  fprintf(object, "Lexeme = \"%s\", lookahead = \"%d\".\n", lexeme, lookahead);
}

// função que pode ser chamada externamente para ativar o flag de erro
void exit_with_error(int status) {  
  error = 1;
  print_status_message(status);
}