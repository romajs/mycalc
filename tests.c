#include <tests.h>

test(const char* expression, int expected) {
   fprintf(object, "Testing: %s, expeced: %d.\n", expression, expected);
	fprintf(source, "%s\n", expression);
   rewind(source);
   int result = 0;//mycalc();
   fprintf(object, "result = %d\n", result);
	if(result == expected) {
		fprintf(object, "OK.\n");
	} else {
		fprintf(object, "Failed.\n");
	}
}

do_tests(void) {
	fprintf(object, "Initiating tests...\n");	
	source = fopen("test", "rw"); //object;
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