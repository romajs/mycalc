#include <parser.h>



/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 * expr -> [-] term { [+|-] term }
 *
 *
 *
 *             -------(+|-)-------
 *             |                 |
 *             v                 |
 * (expr)--->(init)--->(term)---(?)-->(end)
 *
 */
void expr(void)
{
  
  int signal = 0;
  int lastop = 0;
  if(lookahead=='-'){
    /*SA 1*/signal = 1;/**/
    match('-');
  }
init:	
  term();
  /*SA 2*/if(signal){
    fprintf(object," +/- ");
    signal = 0;
  }/**/
  /*SA 3*/if(lastop){
    T_lvl-=2;
    fprintf(object," %c ", lastop);
    lastop = 0;
  }/**/
  if(lookahead == '+' || lookahead == '-') {
    lastop = lookahead;
    match(lookahead);  
    goto init;
  }
}
/*
 * term -> fact { [*|/] fact }
 *
 *             -------(*|/)-------
 *             |                 |
 *             v                 |
 * (term)--->(init)--->(fact)---(?)-->(end)
 *
 */
void term(void)
{


  T_lvl++;
  int lastop = 0;
init:	
  fact();
  /*SA 3*/if(lastop){
    T_lvl--;
    fprintf(object," %c ", lastop);
    lastop = 0;
  }/**/
  if(lookahead == '*' || lookahead == '/') {
    lastop = lookahead;
    match(lookahead);
    goto init;
  }
}
/*
 * fact -> ID
 *       | NUM
 *       | ( expr )
 *
 */
void fact(void)
{
  F_lvl++;
  switch(lookahead){
    case ID:
      fprintf(object," %s ", lexeme);
      if(match(ID)) F_lvl--;	
      break;
    case NUM:
      fprintf(object," %s ", lexeme);
      if(match(NUM)) F_lvl--;	
      break;
    default:
      if(match('(')) E_lvl++;
      expr();
      if(match(')')) { E_lvl--; F_lvl--; }
      break;
  }
}

