#include <parser.h>

int E_lvl; 										// contador de parêntese (qtd de recursões em E)

#define	MAX_STACK_SIZE		0x10000
double operand[MAX_STACK_SIZE]; 			// pilha de operandos (ID | CTE)
int sp = -1;
int oper[MAX_STACK_SIZE]; 					// pilha de operadores
int opsp = -1;

#define	MAX_MEM_SIZE		0x10000
double acc[MAX_MEM_SIZE]; 					// acumulador ???

#define	MAX_SYM_TAB				1024
#define	MAX_ID_LEN				  32
char SYMTAB[MAX_SYM_TAB][MAX_ID_LEN]; 	// tabela de símbolos
int nextentry = -1; 							// posição da tabela (próxima entrada)

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

// busca e retorna o valor de uma determinada variável
double recall(char const *variable) {
  int i;
  for(i = 0 ; i < nextentry; i++) {
	 if(strcmp(SYMTAB[i], variable) == 0)
		return acc[i];
  }
  strcpy(SYMTAB[nextentry++], variable);
  return acc[i];
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
void expr(void)
{
  int chs = 0; // flag de inversão de sinal
  
  E:
  if(lookahead == '-') { // inversão de sinal
	 chs = 1;
    match('-');
  }
  
  T:
  
  F:
  if(lookahead == ID) {
    operand[++sp] = recall(lexeme); // empilha o valor da varíavel
    match(ID);
  } else if(lookahead == NUM) {
    operand[++sp] = atof(lexeme); // empilha o valor da constante
    match(NUM);
  } else {
	 E_lvl++;
    match('(');
    goto E;
  }
  
  _F:
  if(opsp > -1) { // calcula se houver operador na pilha
	 operand[--sp] = calc(operand[sp+1], operand[sp], oper[opsp--]);	 
  }
  if(lookahead == '*'||lookahead == '/') {
	 oper[++opsp] = lookahead;
    match(lookahead);
    goto F;
  }
  
  _T:
  
  if(chs) { // inverte o sinal no topo da pilha de operadores
	 chs = 0;
	 operand[sp] = -operand[sp];
  }
  
  if(opsp > -1) { // calcula se houver operador na pilha
	 operand[--sp] = calc(operand[sp+1], operand[sp], oper[opsp--]);	 
  }
  if(lookahead == '+'||lookahead == '-') {
	 oper[++opsp] = lookahead;	 
    match(lookahead);
    goto T;
  }
  _E:
  if(E_lvl) {
	 E_lvl--;
	 match(')');
	 goto _F;
  }
  
  match('\n'); // EOF
  return;
}

