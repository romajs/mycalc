#include <parser.h>

#define	MAX_STACK_SIZE		0x1000			
#define MAX_RECURSION_SIZE			0x100			
double operand[MAX_STACK_SIZE]; 								// pilha de operandos (ID | CTE)
int sp = -1;														
int oper[MAX_RECURSION_SIZE][MAX_STACK_SIZE];		// pilha de operadores
int opsp = -1;
int can_oper = 0;																// permite a execução de operações
int E_lvl = -1, T_lvl = -1, F_lvl = -1, A_lvl = -1;				// contadores de recursão

#define	MAX_SYM_TAB				0x1000							
#define	MAX_ID_LEN				    32 									// obs: já definido em tokens.h
char SYMTAB[MAX_SYM_TAB][MAX_ID_LEN]; 					// tabela de símbolos (armazenamento variávies)
int nextentry = -1; 														// posição da tabela (próxima entrada)

//#define	MAX_MEM_SIZE		0x1000				// Dúvida: Eraldo colocou isso ? mas não é ligada a SYMTAB?
double acc[MAX_SYM_TAB]; 											// pilha de valores da tabela de símbolos

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

void debug_symtab(void) {
  fprintf(debug, "SYMTAB size=\"%d\"\n", nextentry + 1);
  int i;
  for(i = 0; i <= nextentry; i++) {
		fprintf(debug, " [%d] %s = %.2f\n", i, SYMTAB[i], acc[i]);
	}
}

// busca uma variável na tabela de símbolos e retorna sua posição
// caso não existe, então define
double recall(char const *variable) {
  int i;
  for(i = 0; i <= nextentry; i++) {
		if(strcmp(SYMTAB[i], variable) == 0)
			return i;
	}
  if(nextentry + 1 > MAX_SYM_TAB) {
    exit_with_error(STACK_MEM_OVERFLOW);
  }  
  strcpy(SYMTAB[++nextentry], variable); // adiciona a variável em SYMTAB
  return i;
}

// busca uma variável na tabela de símbolos e retorna seu valor de acc
double getvalue(char const *variable) {
  int i;
  for(i = 0; i <= nextentry; i++) {
		if(strcmp(SYMTAB[i], variable) == 0)
			return acc[i];
	}
  exit_with_error(ID_NOT_DECLARED);
  //exit(ID_NOT_DECLARED);
}

// função que empilha um 'operando' na pilha de operandos
void push_operand(double value) {
	operand[++sp] = value;
	fprintf(debug, "(push) operand[%d] = %.2f\n", sp, operand[sp]);	
}

// função que empilha um 'operador' na pilha de operadores
void push_oper(token_t token) {
	oper[E_lvl][++opsp] = token;
	fprintf(debug, "(push) oper[%d] = %c\n", opsp, oper[E_lvl][opsp]);	
}

// função que verifica se é possível operar no momento
should_oper(void) {
	// pode operar desde que:
	//	1 - enquanto houverem operadoes;
	//	2 - enquanto o próximo operador não for '*' ou '/';
	return oper[E_lvl][opsp]  && !(lookahead == '*' || lookahead == '/');
}

// função que executa as operações da pilha (desde que possa operar)
void exec_oper(void) {
	if(opsp > - 1 && oper[E_lvl][opsp] && can_oper) { // se puder operar
		do {			
			fprintf(debug, "(pop) oper[%d] = %c\n", opsp, oper[E_lvl][opsp]);	
			operand[--sp] = calc(operand[sp], operand[sp+1], oper[E_lvl][opsp--]);
			fprintf(debug, "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
		} while(can_oper = should_oper());	// enquanto puder operar
	} 
}

/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 *
 * expr -> [attr] [-] term { [+|-] term }
 *
 *                         --(+|-)--
 *        -(A)-   -(-)-    |       |
 *        |   |   |   |    |       |
 *        |   v   v   |    v       |
 * (E)------------------->(T)--------->(_E)
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
 * fact -> ID ['=' expr] | NUM | '('expr')'
 *
 * (F)---------------> (ID) --------------\
 *  | \                                    \
 *  |  \------------> (NUM) ------------->(_F)
 *  |                                      /
 *   \-----> '(' ----> (E) ----> ')' -----/
 * 
 *
 * attr -> ID '='
 *
 * (A)------>(ID)------>'='------>(_A)
 *
 */
double expr(void)
{
  double value;
  int attr = -1; // posição da atribuição (se existir)
	E_lvl = -1, T_lvl = -1, F_lvl = -1, A_lvl = -1;
	sp = -1, opsp = -1;
  
  A: fprintf(debug, "A: %d\n", ++A_lvl);
  
  // OBS: por enquanto suporta atribuição única (por vez)
  if(lookahead == ID) {
    char temp[MAX_ID_LEN]; strcpy(temp, lexeme); // salva temporariamente o lexeme
    match(ID);   
    if(lookahead == '=') { // atribuição      
      match('=');
      attr = recall(temp);      
      fprintf(debug, "\"%s\" foi adicionado a SYMTAB em %d.\n", temp, attr); 
      // acc ainda não tem valor (acc = 0) mas não importa no momento
    } else {
      unmatch(ID, temp);
    }
  }
    
  _A: fprintf(debug, "A: %d\n", --A_lvl);
	
  E: fprintf(debug, "E: %d\n", ++E_lvl);
  
	can_oper = 0;
	
  if(lookahead == '-') { // inversão de sinal
		match('-');
    fprintf(debug, "(signal reversion) activated.\n");
		push_operand(0);
		push_oper('-');
		can_oper = 1; // deve fazer esta operação o quanto antes (imediato)    
  }
  
  T: fprintf(debug, "T: %d\n", ++T_lvl);
  
  F: fprintf(debug, "F: %d\n", ++F_lvl);
  
  if(lookahead == ID) {
    push_operand(getvalue(lexeme)); // empilha o valor da variável de SYMTAB
    match(ID);    
  } else if(lookahead == NUM) {
    push_operand(atof(lexeme)); // empilha o valor da constante
    match(NUM);
  } else if(lookahead == '(') {
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

  // operações de '+' ou '-' são 'agendadas'
	// Não precisa atribuir 'can_oper = 0;' pois já é feito automaticamente em exe_oper() 
 
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
  
  value = operand[sp--];
  
  if(attr > -1) {
    acc[attr] = value;
  }
  
	debug_symtab();
  
  return value;
}