fe: fe.c ../lib/$(LIB)
	$(CC) -O2 -DLIB=\"$(LIB)\" -DVER=\"$(VER)\" -Wall -Werror \
              -DLIBDIR=\"$(LIBDIR)\" -DBINDIR=\"$(BINDIR)\" \
	      -o fe fe.c -L../lib -l$(FE)

install: $(BINDIR)/fe
$(BINDIR)/fe: fe
	mkdir -p $(BINDIR)
	install fe $(BINDIR)

clean:
	rm -rf fe fe.dSYM/
