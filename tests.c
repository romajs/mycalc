#include <tests.h>

int total = 0;
int done = 0;

test(const char* expression, double expected) {
   fprintf(object, "Testing: \"%s\", expeced: \"%.2f\";\n", expression, expected); 
   source = fopen("test.txt", "w+");
   fprintf(source, "%s\n", expression);
   rewind(source);
      
   pid_t child;
   int status = -1; 
   
   // copiado de 'mycalc.c' e adaptado
   if(!(child = fork())) { // inicia novo processo
      //fprintf(object, "child: %d\n", child);	
        
      
      /* get the first token to begin the parsing */
      lookahead = gettoken(source);
      
      /* call the grammar initial symbol */	
      double value = 0;
      value = expr();
      fprintf(object, " -> Found = %.2f\n", value);
      
      if(value == expected) {
         exit(OK);
      } else {
         exit(FAIL);
      }
   } else if (child == -1) {
      fprintf(object, "fork error.");
      exit(EXIT_FAILURE);
   } else {			
      //fprintf(object, "waiting for expr...\n");
      waitpid(child, &status, WUNTRACED);	// aguarda o filho terminar	
   }	 

 
   if(WEXITSTATUS(status) == OK) {
      done++;
      fprintf(object, "OK.\n");
   } else {
      fprintf(object, "Failed. \"%d\"\n", WEXITSTATUS(status));
   }
   total++;
   fclose(source);
}

do_tests(void) {
	fprintf(object, "Initiating tests...\n");	
	test("1+2", 3);
	test("1+2+3*4", 15);
	test("1+2*3+4", 11);
	test("1+2*3*4", 25);
	test("1*2+3+4", 9);
	test("1*2+3*4", 14);
	test("1*2*3+4", 10);
	test("1*2*3*4", 24);
  test("(2+3)*5", 25);
  test("5*(2+3)", 25);
	test("1/4", 0.25);
  test("1-2*3*4+(1-1)", -23);
  test("1-2*3*4*(2-2)", 1);
	test("1-1", 0);
  test("1-2", -1);
  test("2-1", 1);
  test("-(1+1)", -2);
	test("(-1+1)", 0);	
	test("1-(1-1*(-1+1))", 0);
  test("5-(1+1)", 3);
	test("5-((-1+1)+(1+2*3*4+(5/6)-7))", 5-((-1+1)+(1+2*3*4+(5.00/6.00)-7)));
	//test("", 0.00);
	source = stdin;  
  double percent = (double) done / total * 100;
  fprintf(object, "Total = %d, Done = %d, Failed = %d. (%.1f%%)\n", total, done, total - done, percent);
}