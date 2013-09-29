#include <parser.h>

// função que calcula o resultado entre duas variávies dado seu operador
double calc(double x, double y, int op) { 
  double result = 0.00;
  switch(op) {
		case '+': result = x + y; break;
		case '-': result = x - y; break;
		case '*': result = x * y; break;
		case '/': result = x / y; break;
  }
  debug( "(calc) %.2f %c %.2f = %.2f\n", x, op, y, result);
  return result;
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
  //strcpy(SYMTAB[++nextentry], variable); // adiciona a variável em SYMTAB
  return -1;
}

// busca uma variável na tabela de símbolos e retorna seu valor de acc
double getvalue(char const *variable) {
  int i;
  for(i = 0; i <= nextentry; i++) {
		if(strcmp(SYMTAB[i], variable) == 0)
			return acc[i];
	}
  //exit_with_error(ID_NOT_DECLARED);
  //exit(ID_NOT_DECLARED);
  return 0.00;
}

// função que empilha um 'operando' na pilha de operandos
void push_operand(double value) {
	operand[++sp] = value;
	debug( "(push) operand[%d] = %.2f\n", sp, operand[sp]);	
}

// função que empilha um 'operador' na pilha de operadores
void push_oper(token_t token) {
	oper[E_lvl][++opsp] = token;
	debug( "(push) oper[%d] = \"%c\"\n", opsp, oper[E_lvl][opsp]);	
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
			if(oper[E_lvl][opsp] == '=') {
        debug( "(pop) oper[%d] = \"%c\"\n", opsp, oper[E_lvl][opsp]);	
        double value = operand[sp--];
        int attr_pos = operand[sp];
        debug("attr_pos = %d, lexeme = \"%s\".\n", attr_pos, SYMTAB[attr_pos]);
        acc[attr_pos] = operand[sp] = value;
        debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
        opsp--;
      } else {
        debug( "(pop) oper[%d] = \"%c\"\n", opsp, oper[E_lvl][opsp]);	
        operand[--sp] = calc(operand[sp], operand[sp+1], oper[E_lvl][opsp--]);
        debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
      }
    } while(can_oper = should_oper());	// enquanto puder operar
	} 
}

/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 *
 * expr -> {attr} [-] term { [+|-] term }
 *
 *                         --(+|-)--
 *        -(A)-   -(-)-    |       |
 *        |   |   |   |    |       |
 *        v   |   v   |    v       |
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
  memset(&oper[0], 0, sizeof(oper));
	memset(&operand[0], 0, sizeof(operand));	
	E_lvl = -1, T_lvl = -1, F_lvl = -1, A_lvl = -1;
	sp = -1, opsp = -1;
  int can_attr = 0; // flag de atribuição
	
  E: debug( "E: %d\n", ++E_lvl);
  
	can_oper = 0;
  can_attr = 1;
	
  if(lookahead == '-') { // inversão de sinal
		match('-');
    debug( "(signal reversion) activated.\n");
		push_operand(0);
		push_oper('-');
		can_oper = 1; // deve fazer esta operação o quanto antes (imediato)    
  }
  
  T: debug( "T: %d\n", ++T_lvl);
  
  F: debug( "F: %d\n", ++F_lvl);  
  
  A: debug( "A: %d\n", ++A_lvl);
  char variable[MAX_ID_LEN];
  int attr_pos = -1;
  
  if(lookahead == ID) {
    strcpy(variable, lexeme);
    if((attr_pos = recall(variable) ) > -1) {
      push_operand(getvalue(variable)); // empilha o valor da variável de SYMTAB
    }
    match(ID);    
  } else if(lookahead == NUM) {
    push_operand(atof(lexeme)); // empilha o valor da constante
    match(NUM);
  } else if(lookahead == '(') {
    match('(');
    goto E;
  }    
  
  if(lookahead == '=' && can_attr) { // atribuição
    if(attr_pos == -1) {
      strcpy(SYMTAB[++nextentry], variable);
      attr_pos = nextentry;
    }
    debug( "(attr) \"%s\" foi adicionado a SYMTAB em %d.\n", variable, attr_pos);  
    push_operand(attr_pos);
    push_oper('=');       
    match('=');    
    goto A;
  }
    
  _A: debug( "A: %d\n", --A_lvl);
  
  can_attr = 0;
  
  _F: debug( "_F: %d\n", --F_lvl);
      
	exec_oper(); // executa as operações mais 'voláteis' ('*' & '/')	 
  
  if(lookahead == '*' || lookahead == '/') {
	  push_oper(lookahead);
    match(lookahead);
		can_oper = 1;
    goto F;
  }
  
  _T: debug( "_T: %d\n", --T_lvl);

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
  
  _E: debug( "_E: %d\n", --E_lvl);
  
  if(E_lvl > -1) {
	  match(')');
    goto _F;
  }
  
  match(EOF); 
	
	debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);
  return operand[sp--];
}