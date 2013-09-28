#include <debug.h>

FILE *out;

void set_debug(FILE *f) {
  out = f;
}

void debug(const char *format, ...) {
  if(!out) return;
  va_list arg;
  va_start(arg, format);
  vfprintf(out, format, arg);
  va_end(arg);
}

void debug_symtab(const char table[MAX_SYM_TAB][MAX_ID_LEN], const double *value, int size) {
  if(!out) return;
  debug( "| SYMTAB | size = %d |\n", size);
  int i;
  debug( "| I | ADDR \t| VALUE \t| LEXEME \t|\n");
  for(i = 0; i < size; i++) {
		debug( "| %d | %d \t| %.2f \t| %s \t| \n", i, table[i],
      value[i], table[i]);
	}
}