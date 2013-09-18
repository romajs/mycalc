#include <parser.h>

#define	MAX_STACK_SIZE		0x10000
double operand[MAX_STACK_SIZE]; 			// pilha de operandos (ID | CTE)
int sp = -1;
int oper[MAX_STACK_SIZE]; 					// pilha de operadores
int opsp = -1;


#define	MAX_SYM_TAB				1024
#define	MAX_ID_LEN				  32 // já definido em tokens.h
char SYMTAB[MAX_SYM_TAB][MAX_ID_LEN]; 	// tabela de símbolos (armazenamento variávies)
int nextentry = -1; 							// posição da tabela (próxima entrada)

#define	MAX_MEM_SIZE		0x10000
double acc[MAX_MEM_SIZE]; 					// pilha de valores da tabela de símbolos

// função que calcula o resultado entre duas variávies dado seu operador
double calc(double x, double y, int op) { 
  double result = 0.00;
  switch(op) {
	 case '+': result = x + y; break;
	 case '-': result = x - y; break;
	 case '*': result = x * y; break;
	 case '/': result = x / y; break;
  }
  fprintf(debug, "(calc) %.2f %c %.2f = %.2f\n", x, op, y, result);
  return result;
}

// busca uma variável na tabela de símbolos e retorna seu valor
double recall(char const *variable) {
  int i;
  for(i = 0; i < nextentry; i++) {
		if(strcmp(SYMTAB[i], variable) == 0)
			return acc[i];
	}
  strcpy(SYMTAB[nextentry++], variable);
  return acc[i];
}

double revert_signal() {
	operand[sp] = -operand[sp];
  fprintf(debug, "(revert) operand[%d] = %.2f\n", sp, operand[sp]);	
}

void push_operand(double value) {
	operand[++sp] = value;
	fprintf(debug, "(push) operand[%d] = %.2f\n", sp, operand[sp]);	
}

void push_oper(token_t token) {
	oper[++opsp] = token;
	fprintf(debug, "(push) oper[%d] = %c\n", opsp, oper[opsp]);	
}

int queue = 0;

double unqueue() {
	fprintf(debug, "unqueue: %d\n", queue);	
  if(opsp > -1) {
		queue++; // deve incrementar ao menos uma vez (já que chegou até aqui)
		if((oper[opsp] == '*' || oper[opsp] == '/') || lookahead == EOF) {
			do {			
				fprintf(debug, "(pop) oper[%d] = %c\n", opsp, oper[opsp]);	
				operand[--sp] = calc(operand[sp+1], operand[sp], oper[opsp--]);
				fprintf(debug, "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
				
				fprintf(debug, "queue--: %d\n", --queue);
			} while(queue);	
		} else {
			fprintf(debug, "(queued) oper[%d] = %c\n", opsp, oper[opsp]);	
			fprintf(debug, "queue++: %d\n", queue);
		}
  }
  return operand[sp];
}

int expr(void)
{
  int chs = 0; // flag de inversão de sinal
  int E_lvl = -1, T_lvl = -1, F_lvl = -1;
	queue = 0;
  
  E: fprintf(debug, "E: %d\n", ++E_lvl);
  
  if(lookahead == '-') { // inversão de sinal
	  chs = 1;
    fprintf(debug, "signal reversion activated!\n");
    match('-');
  }
  
  T: fprintf(debug, "T: %d\n", ++T_lvl);
  
  F: fprintf(debug, "F: %d\n", ++F_lvl);
  
  if(lookahead == ID) {
    push_operand(recall(lexeme)); // empilha o valor da variável
    match(ID);
  } else if(lookahead == NUM) {
    push_operand(atof(lexeme)); // empilha o valor da constante
    match(NUM);
  } else {
    match('(');
    goto E;
  }  
  
  _F: fprintf(debug, "_F: %d\n", --F_lvl);
      
	unqueue();	 
  
  if(lookahead == '*'||lookahead == '/') {
	  push_oper(lookahead);
    match(lookahead);
    goto F;
  }
  
  _T: fprintf(debug, "_T: %d\n", --T_lvl);
  
  if(chs) { // se houver sinal    
    chs = 0;
    revert_signal(); // inverte
  }
 
  if(lookahead == '+'||lookahead == '-') {
    push_oper(lookahead);
    match(lookahead);
    goto T;
  }
  
  _E: fprintf(debug, "_E: %d\n", --E_lvl);
  
  if(E_lvl > -1) {
	  match(')');
    goto _F;
  }
  
  match(EOF); 
	
	fprintf(debug, "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	 
	
  return operand[sp--];
}
/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 * expr -> [-] term { [+|-] term }
 *
 *
 *
 *                  --(+|-)--
 *        -(-)-     |       |
 *        |   |     |       |
 *        |   v     v       |
 * (E)------------>(T)--------->(_E)
 *
 * 
 * term -> fact { [*|/] fact }
 *
 *             --(*|/)--
 *             |       |
 *             v       |
 * (T)------->(F)--------->(_T)
 * 
 * 
 * fact -> ID | NUM | '('expr')'
 *
 * (F)---------------> (ID) -------------\
 *  | \                                  v
 *  |  -------------> (NUM) ----------->(_F)
 *  |                                    ^
 *  \------> '(' ----> (E) ----> ')' ----/
 *
 */