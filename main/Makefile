fe: fe.c ../lib/$(LIB)
	$(CC) $(OPTFLAG) -DLIB=\"$(LIB)\" -DVER=\"$(VER)\" -Wall -Werror \
              -DLIBDIR=\"$(LIBDIR)\" -DBINDIR=\"$(BINDIR)\" \
	      -DMAGIC=\"$(MAGIC)\" \
	      -o fe fe.c -L../lib -l$(FE)

install: $(BINDIR)/fe
$(BINDIR)/fe: fe ../Makefile
	mkdir -p $(BINDIR)
	install fe $(BINDIR)

clean:
	rm -rf fe fe.dSYM/
