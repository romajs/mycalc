#include <tests.h>

int total = 0;
int done = 0;

test(const char* expression, double expected) {
   fprintf(object, "Testing: \"%s\", expeced: \"%.2f\";\n", expression, expected); 
   source = fopen("test.txt", "w+");
   fprintf(source, "%s\n", expression);
   rewind(source);

    error = 0;
    lookahead = -1;
    lexeme[0] = 0;
    //if(!(child = fork())) { // inicia novo processo
       //fprintf(object, "child: %d\n", child);			  
    
       /* get the first token to begin the parsing */
       lookahead = gettoken(source);
       
       /* call the grammar initial symbol */
       double value = expr();
		fprintf(object, " = %.2f\n", value);
       if(!error && value == expected) {        
	printf("OK.\n" );
	done++;	
       } else {
	printf("Failed.\n" );
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
