enum { // TOKENS
	ID = 1025,
	NUM,
};

enum { // RETURN STATUS (ERRORS)
	OK = 0,
   FAIL,
	TOKEN_MISMATCH,
};

#define         MAX_ID_LEN      32
#define         MAX_DEC_LEN     10
