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

#define   MAX_ID_LEN              32	// Tamanho m�ximo dos identificadores
#define   MAX_INT_DIG             10	// N�mero m�ximo de d�gitos de um inteiro
#define	  MAX_PRC_DIG		          17	// N�mero m�ximo de d�gitos de precis�o de ponto flutuante

#define	  MAX_STACK_SIZE		   0x100  // Tamanho m�ximo das pilhas			
#define   MAX_RECURSION_SIZE   0x100  // N�mero m�ximo de recurs�es	

#define	  MAX_SYM_TAB				   0x100  // Tamanho m�ximo de symtab				
#define	  MAX_MEM_SIZE		     0x100	// D�vida: Eraldo colocou isso ? mas n�o � ligada a SYMTAB?			

#endif // TOKENS_H