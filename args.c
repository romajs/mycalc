#include <args.h>

int D_FLAG;

void match_args(int argc, char *argv[]) {
  int i; // manipulação de parâmetros
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
    debug = object;
  } else {
		debug = fopen("debug.txt", "w");
	}
  if(T_FLAG) {
   do_tests();
  }
}
