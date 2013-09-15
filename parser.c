#include <parser.h>

int E_lvl; 										// contador de parêntese (qtd de recursões em E)

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
  switch(op) {
	 case '+': return x + y;
	 case '-': return x - y;
	 case '*': return x * y;
	 case '/': return x / y;
  }
  return 0.00;
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

double reverse_signal() {
	operand[sp] = -operand[sp];
  fprintf(debug, "(reverse) operand[%d] = %.2f\n", sp, operand[sp]);	
}

void push_operand(double value) {
	operand[++sp] = value;
	fprintf(debug, "(push) operand[%d] = %.2f\n", sp, operand[sp]);	
}

void push_oper(token_t token) {
	oper[++opsp] = token;
	fprintf(debug, "(push) oper[%d] = %c\n", opsp, oper[opsp]);	
}

double pop() {
  //if(opsp > -1 && ( oper[opsp] == '*' || oper[opsp] == '/')) {
  fprintf(debug, "(pop) oper[%d] = %c\n", opsp, oper[opsp]);	
  operand[--sp] = calc(operand[sp+1], operand[sp], oper[opsp--]);
  fprintf(debug, "(pop)  operand[%d] = %.2f\n", sp, operand[sp]);	  
  //}
  return operand[sp];
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
int expr(void)
{
  int chs = 0; // flag de inversão de sinal
  E_lvl = 0;
  
  E: fprintf(debug, "E:\n");
  if(lookahead == '-') { // inversão de sinal
	  chs = 1;
    fprintf(debug, "signal reverse activated.\n");
    match('-');
  }
  
  T: fprintf(debug, "T:\n");
  
  F: fprintf(debug, "F:\n");
  
  if(lookahead == ID) {
    push_operand(recall(lexeme)); // empilha o valor da variável
    match(ID);
  } else if(lookahead == NUM) {
    push_operand(atof(lexeme)); // empilha o valor da constante
    match(NUM);
  } else {
	 E_lvl++;
    match('(');
    goto E;
  }
  
  _F: fprintf(debug, "_F:\n");
  if(opsp > -1 && ( oper[opsp] == '*' || oper[opsp] == '/')) {
	  pop();	 
  }
  if(lookahead == '*'||lookahead == '/') {
	  push_oper(lookahead);
    match(lookahead);
    goto F;
  }
  
  _T: fprintf(debug, "_T:\n");
  
  if(chs) { // se houver sinal    
    chs = 0;
    reverse_signal(); // inverte
  }
  
  if(opsp > -1 && ( oper[opsp] == '+' || oper[opsp] == '-')) {
	  pop();	 
  }
  if(lookahead == '+'||lookahead == '-') {
    push_oper(lookahead);
    match(lookahead);
    goto T;
  }
  _E: fprintf(debug, "_E:\n");
  if(E_lvl) {
    E_lvl--;
	  match(')');
    goto _F;
  }
  
  match(EOF);
  return operand[sp--];
}

