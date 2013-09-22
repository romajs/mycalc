#include <parser.h>

#define	MAX_STACK_SIZE		0x1000			
#define MAX_RECURSION_SIZE			0x100			
double operand[MAX_STACK_SIZE]; 								// pilha de operandos (ID | CTE)
int sp = -1;														
int oper[MAX_STACK_SIZE][MAX_RECURSION_SIZE];		// pilha de operadores
int opsp = -1;
int can_oper = 0;																// permite a execução de operações
int E_lvl = -1, T_lvl = -1, F_lvl = -1; 				// contadores de recursão

#define	MAX_SYM_TAB				1024							
#define	MAX_ID_LEN				  32 									// obs: já definido em tokens.h
char SYMTAB[MAX_SYM_TAB][MAX_ID_LEN]; 					// tabela de símbolos (armazenamento variávies)
int nextentry = -1; 														// posição da tabela (próxima entrada)

#define	MAX_MEM_SIZE		0x10000									//
double acc[MAX_MEM_SIZE]; 											// pilha de valores da tabela de símbolos

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
	oper[E_lvl][++opsp] = token;
	fprintf(debug, "(push) oper[%d] = %c\n", opsp, oper[E_lvl][opsp]);	
}

token_t pop_oper() {
	fprintf(debug, "(pop) oper[%d] = %c\n", opsp, oper[E_lvl][opsp]);	
	return oper[E_lvl][opsp--];
}

// função que verifica se é possível operar no momento
should_oper() {
	// pode operar desde que:
	//	1 - enquanto houverem operadoes;
	//	2 - enquanto o próximo operador não for '*' ou '/';
	return oper[E_lvl][opsp]  && !(lookahead == '*' || lookahead == '/');
}

// função que executa as operações da pilha, desde que possa operar
double exec_oper() {
  if(opsp > - 1 && oper[E_lvl][opsp]) { // verifica se há operadores da pilha
	
		if(can_oper) { // se puder operar
			do {			
				fprintf(debug, "(pop) oper[%d] = %c\n", opsp, oper[opsp]);	
				operand[--sp] = calc(operand[sp], operand[sp+1], oper[E_lvl][opsp--]);
				fprintf(debug, "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
			} while(can_oper = should_oper());	// enquanto puder operar
		} 
  }
  return operand[sp]; // retorna o operando calculado (popado)
}

int expr(void)
{
  int chs = 0; // flag de inversão de sinal
	E_lvl = -1, T_lvl = -1, F_lvl = -1;
	sp = -1, opsp = -1;
	
  E: fprintf(debug, "E: %d\n", ++E_lvl);
	
	can_oper = 0;
	
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
      
	exec_oper(); // executa as operações mais 'voláteis' ('*' & '/')	 
  
  if(lookahead == '*' || lookahead == '/') {
	  push_oper(lookahead);
    match(lookahead);
		can_oper = 1;
    goto F;
  }
  
  _T: fprintf(debug, "_T: %d\n", --T_lvl);
  
  if(chs) { // se houver sinal    
    chs = 0;
    revert_signal(); // inverte
  }
 
  can_oper = 0; // operações de '+' ou '-' são 'agendadas'
 
  if(lookahead == '+' || lookahead == '-') {
    push_oper(lookahead);
    match(lookahead);
    goto T;
  }
	
	// se chegou até É porque OU é fim de arquivo OU é ')' então deve operar o restante da pilha
	can_oper = 1;
	exec_oper();	
  
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