#include <tests.h>

#define SOURCE_FNAME "tests.mycalc.txt"

int total = 0; // total de testes executados
int done = 0; // total de testes sucedidos

// função que executa um teste específico
// 1 - faz uma chamada padrão a 'mycalc
// 2 - compara o valor retornado com o valor esperado
// obs: utiliza um arquivo temporário (SOURCE_FNAME)
// para redirecinar a entrada à 'source'
test(const char* expression, double expected) {

	fprintf(object, "[Testing]: \"%s\", expeced: \"%.2f\";\n", expression, expected); 
	
  source = fopen(SOURCE_FNAME, "w+");
  
	fprintf(source, "%s\n", expression); // escreve/imprime a expressão no arquivo
	rewind(source); // retornar a posição da cabeça para o início do arquivo
  
	double value; // valor de retorno  
  if(mycalc(&value) && value == expected) { // chama 'mycalc' e compara com o valor experado        
		fprintf(object, "OK!\n" );
		done++;	
	} else {
		fprintf(object, "FAILED! Found = \"%.2f\";\n", value); 
	}
  
	total++;
	fclose(source);
}

// função que descreve os teste a serem executados
void tests(void) {

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
	test("a=2.23", 2.23);
  test("b=a*2", 4.46);
  //test("", 0.00);
  
  remove(SOURCE_FNAME); // deleta o arquivo temporário
	source = stdin; // redefine 'source' ao padrão de saída (stdin)

  // calcula os totais executados
  double percent = (double) done / total * 100; // porcentagem
  fprintf(object, "Total = %d, Done = %d, Failed = %d. (%.1f%%)\n", total, done, total - done, percent);
}