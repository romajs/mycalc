#include <args.h>

// busca por par�metros/argumentos na chamada de 'mycalc'
// -d: modo debug, toda a execu��o do parser ser� exibida em tela
// -t: testes, ser�o executado alguns testes pr�-definido
// obs: importante o uso dos flags, para permitir qualquer ordem
void match_args(int argc, char *argv[]) {
	int i; // manipula��o de par�metros
	//fprintf(object, "argc: %i\n", argc);
	for(i = 1; i < argc; i++) {
		//fprintf(object, "argv[%i]: %s\n", i, argv[i]);
		if(strcmp(argv[i], "-d") == 0) { // habilita  modo debug
			D_FLAG = 1;
		}
		if(strcmp(argv[i], "-t") == 0) { // executa testes
			T_FLAG = 1;
		}
	}

	if(D_FLAG) {
		set_debug(object);
	}
	
	if(T_FLAG) {
		tests();
	}
}