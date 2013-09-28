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

#define         MAX_ID_LEN      32		// Tamanho m�ximo dos identificadores
#define         MAX_INT_DIG     10		// N�mero m�ximo de d�gitos de um inteiro
#define			MAX_PRC_DIG		17		// N�mero m�ximo de d�gitos de precis�o de ponto flutuante

#endif // TOKENS_H