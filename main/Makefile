fe: fe.c ../lib/libfe.dylib
	$(CC) -O2 -o fe fe.c -L../lib -lfe

install: fe
	mkdir -p $(BINDIR)
	install fe $(BINDIR)

clean:
	rm -rf fe fe.dSYM/
