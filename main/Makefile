C     = $(wildcard *.c)
O     = $(patsubst %.c, %.o, $(C))
AR    = local
LIBAR = liblocal.a

fe: $(LIBAR) ../lib/$(LIB)
	$(CC) -o fe -L. -l$(AR) -L../lib -l$(FE)

$(LIBAR): $(O)
	ar rvs $(LIBAR) $(O)

%.o: %.c
	$(CC) -c $(OPTFLAG) -DLIB=\"$(LIB)\" -DVER=\"$(VER)\" -Wall -Werror \
              -DLIBDIR=\"$(LIBDIR)\" -DBINDIR=\"$(BINDIR)\" \
	      -DMAGIC=\"$(MAGIC)\" $<

usage.o: usage.c ../Makefile

install: $(BINDIR)/fe
$(BINDIR)/fe: fe ../Makefile
	mkdir -p $(BINDIR)
	install fe $(BINDIR)

clean:
	rm -rf fe fe.dSYM/ $(O) $(LIBAR)
