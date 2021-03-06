#include <lexer.h>

token_t lookahead;

char lexeme[MAX_ID_LEN];

// evita caracteres em branco (espa�os)
void skipspaces(FILE *tape)
{
	token_t head;
	while(isspace(head=getc(tape)));
	ungetc(head, tape);
}

// fim de arquivo (EOF) ou ENTER ('\n')
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

// identificadores
token_t isID(FILE *tape)
{
	token_t head;

	if (isalpha(head = getc(tape))) {
		int i = 0;
		lexeme[i++] = head;
		while (isalnum(head = getc(tape))) {
			if(i + 1 < MAX_ID_LEN) lexeme[i++] = head;
		}
		lexeme[i] = 0;
		ungetc(head, tape);
		return ID;
	}
	ungetc(head, tape);
	return 0;
}

// contantes num�ricas: integers ou ponto flutuante (double)
token_t isNUM(FILE *tape)
{
	token_t head;

	if(isdigit(head = getc(tape))) {
		int i = 0;
		lexeme[i++] = head;
		while(isdigit(head = getc(tape))){
			if(i + 1 < MAX_INT_DIG)
				lexeme[i++] = head;		
		}
		if(head == '.') {
			lexeme[i++] = '.';
			while(isdigit(head = getc(tape))){
				if(i + 1 < MAX_PRC_DIG) {
					lexeme[i++] = head;
				} else {
					exit_with_error(MAX_DIG_OVERFLOW);
				}
			}
		}
		lexeme[i] = 0;
		ungetc(head,tape);
		return NUM;
	}
	ungetc(head, tape);
	return 0;
}

// fun��o que l� os caracteres da fita e tenta transformar em um s�mbolo da
// gram�tica, para cada uma das fun��es acima (poss�veis 'tokens')
// caso nenhuma delas seja satisfeita apenas retorna o �ltimo caractere lido.
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
	debug( "lookahead = %d, \"%c\", lexeme = \"%s\"\n", token, token, lexeme);  
	return token;
}
/**************************************************************************
 * parser-to-lexer interface
 */

// fun��o que consome o 'token' atual e obt�m o pr�ximo 'token' da fita
void match(token_t predicted) {
  debug( "lookahead = %d | predicted = %d\n", lookahead, predicted);
  if(lookahead == predicted) {
    if(lookahead != EOF) {
      lookahead = gettoken(source);
    } else {
      debug( "No more tokens in tape to get!\n");
     }
  } else {
    exit_with_error(TOKEN_MISMATCH);
  }
}

// fun��o que devole o 'token' & 'lexeme' atual, e os redefine (volta ao anterior)
void unmatch(token_t previous, const char* temp) {
	debug( "(unmatch) previous = %d, lexeme = \"%s\"\n", lookahead, lexeme);
	int i;
	if(lookahead != EOF) {
		debug( "size(%s) = %u, ungetc: \"", lexeme, strlen(lexeme));
		for(i = strlen(lexeme) - 1; i >= 0; i--) {
			debug( "%c", lexeme[i]);
			ungetc(lexeme[i], source);
		}    
		debug( "\"\n");  
	} else {
		ungetc('\n', source);
	}
	lookahead = previous;
	strcpy(lexeme, temp);
	debug( "lookahead = %d, lexeme = \"%s\"\n", lookahead, lexeme);
}
