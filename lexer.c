#include <lexer.h>

void skipspaces(FILE *tape)
{
	token_t head;
	while(isspace(head=getc(tape)));
	ungetc(head, tape);
}

token_t isID(FILE *tape)
{
	token_t head;

	if (isalpha(head = getc(tape))) {
		while (isalnum(head = getc(tape)));
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
		if(head == '0') {
			return NUM;
		}
		while(isdigit(head = getc(tape)));
		ungetc(head,tape);
		return NUM;
	}
	ungetc(head, tape);
	return 0;
}

token_t gettoken(FILE *tape)
{
	token_t token;

	skipspaces(tape);

	if (token = isID(tape)) return token;

	if (token = isNUM(tape)) return token;

	return getc(tape);
}
/**************************************************************************
 * parser-to-lexer interface
 */

void match(token_t predicted)
{
	if(lookahead == predicted) {
		lookahead = gettoken(source);
	}else{
		fprintf(stderr,"token mismatch\nexiting with error\n");
		exit(-666);
	}
}
