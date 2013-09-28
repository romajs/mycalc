#ifndef TOKENS_H
#define TOKENS_H

enum { // TOKENS
	ID = 1025,
	NUM,
};

enum { // MYCALC RETURN STATUS (ERRORS)
	OK = 0,
	FAIL,
	TOKEN_MISMATCH,
	ID_NOT_DECLARED,
	STACK_MEM_OVERFLOW,
	MAX_DIG_OVERFLOW,
};

#define         MAX_ID_LEN      32		// Tamanho máximo dos identificadores
#define         MAX_INT_DIG     10		// Número máximo de dígitos de um inteiro
#define			MAX_PRC_DIG		17		// Número máximo de dígitos de precisão de ponto flutuante

#endif // TOKENS_H