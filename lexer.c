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
	if((head = getc(tape)) == '\n' || head == EOF) {
      lexeme[0] = 0;
		return EOF;
	}
	ungetc(head, tape);
  return 0;
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
  
  if(token = isEOF(tape)) goto END_TOKEN;
  
  skipspaces(tape);
  
  if (token = isID(tape)) goto END_TOKEN;
  
  if (token = isNUM(tape)) goto END_TOKEN;
  
  token = getc(tape);
	lexeme[0] = token;
  lexeme[1] = 0;  

END_TOKEN:  
  fprintf(debug, "lookahead = %d, \"%c\", lexeme = \"%s\"\n", token, token, lexeme);  
  return token;
}
/**************************************************************************
 * parser-to-lexer interface
 */

void match(token_t predicted) {
  fprintf(debug, "lookahead = %d | predicted = %d\n", lookahead, predicted);
  if(lookahead == predicted) {
    if(lookahead != EOF) {
      lookahead = gettoken(source);
    } else {
      fprintf(debug, "No more tokens in tape to get!\n");
     }
  } else {
    exit_with_error(TOKEN_MISMATCH);
  }
}

void unmatch(token_t previous, const char* temp) {
  fprintf(debug, "(unmatch) previous = %d, lexeme = \"%s\"\n", lookahead, lexeme);
  int i;
  if(lookahead != EOF) {
    fprintf(debug, "size(%s) = %u, ungetc: \"", lexeme, strlen(lexeme));
    for(i = strlen(lexeme) - 1; i >= 0; i--) {
      fprintf(debug, "%c", lexeme[i]);
      ungetc(lexeme[i], source);
    }    
    fprintf(debug, "\"\n");  
  } else {
    ungetc('\n', source);
  }
  lookahead = previous;
  strcpy(lexeme, temp);
  fprintf(debug, "lookahead = %d, lexeme = \"%s\"\n", lookahead, lexeme);
}
