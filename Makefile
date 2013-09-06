CFLAGS=-I. -g

relloc=lexer.o mycalc.o parser.o

mycalc: $(relloc)
	$(CC) -o $@ $^

clean:
	$(RM) $(relloc)
mostlyclean: clean
	$(RM) mycalc *~
