#include <lexer.h>

void skipspaces(FILE *tape)
{
  token_t head;
  while(isspace(head=getc(tape)));
  ungetc(head, tape);
}

token_t isEOF(FILE *tape)
{
	token_t head;
	if((head=getc(tape))== 10) {
		return head;
	}
	ungetc(head, tape); return 0;
}

token_t isID(FILE *tape)
{
  token_t head;
  
  if (isalpha(head = getc(tape))) {
    int i = 0;
    lexeme[i++] = head;
    while (isalnum(head = getc(tape))){
      if(i + 1 < MAX_ID_LEN) lexeme[i++] = head;
    }
    lexeme[i] = 0;
    ungetc(head, tape);
    return ID;
  }
  ungetc(head, tape);
  return 0;
}

token_t isNUM(FILE *tape)
{
  token_t head;
  
  if(isdigit(head = getc(tape))) {
    int i = 0;
    if(head == '0') {
      lexeme[0] = head;
      lexeme[1] = 0;
      return NUM;
    }
    lexeme[i++] = head;
    while(isdigit(head = getc(tape))){
      if(i + 1 < MAX_DEC_LEN) lexeme[i++] = head;
    }
    lexeme[i] = 0;
    ungetc(head,tape);
    return NUM;
  }
  ungetc(head, tape);
  return 0;
}

token_t gettoken(FILE *tape)
{
  token_t token;
  
  if(token = isEOF(tape)) return token;
  
  skipspaces(tape);
  
  if (token = isID(tape)) return token;
  
  if (token = isNUM(tape)) return token;
  
  lexeme[1] = 0;  
  return lexeme[0] = getc(tape);
}
/**************************************************************************
 * parser-to-lexer interface
 */

void match(token_t predicted)
{
  //fprintf(stdout, "\nlookahead: %d | predicted: %d\n", lookahead, predicted);
  if(lookahead == predicted) {
	if(lookahead != 10)
		lookahead = gettoken(source);
  }else{
    fprintf(stderr,"token mismatch\nexiting with error\n");
    exit(-666);
  }
}
