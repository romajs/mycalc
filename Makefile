CFLAGS=-I. -g

relloc=lexer.o parser.o mycalc.o args.o tests.o debug.o

mycalc: $(relloc)
	$(CC) -o $@ $^

clean:
	$(RM) $(relloc)
mostlyclean: clean
	$(RM) mycalc *~
