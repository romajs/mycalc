#include <tests.h>

test(const char* expression, int expected) {
   fprintf(object, "Testing: \"%s\", expeced: \"%d\".\n", expression, expected);
	fprintf(source, "%s\n", expression);
   rewind(source);
   static int value;
      
   pid_t child;
   int status = -1;   
   fflush(source);
   
   // copiado de 'mycalc.c' e adaptado
   if(!(child = fork())) { // inicia novo processo
      //fprintf(object, "child: %d\n", child);		
      
      /* get the first token to begin the parsing */
      lookahead = gettoken(source);
      
      /* call the grammar initial symbol */	
      value = expr();
      fprintf(object, " = %d\n", value);
      
      if(value == expected) {
         exit(OK);
      } else {
         exit(FAIL);
      }
      exit(OK);
   } else if (child == -1) {
      fprintf(object, "fork error.");
      exit(EXIT_FAILURE);
   } else {			
      //fprintf(object, "waiting for expr...\n");
      waitpid(child, &status, WUNTRACED);	// aguarda o filho terminar	
   }
 
   if(WEXITSTATUS(status) == OK) {
      fprintf(object, "OK.\n");
   } else {
      fprintf(object, "Failed.\n");
   }
}

do_tests(void) {
	fprintf(object, "Initiating tests...\n");	
	source = fopen("test", "rw"); //object;
	test("1+2", 3);
	test("1+2+3*4", 15);
	test("1+2*3+4", 11);
	test("1+2*3*4", 25);
	test("1*2+3+4", 9);
	test("1*2+3*4", 25);
	test("1*2*3+4", 20);
	test("1*2*3*4", 24) ;
	//test("",);
	source = stdin;
}