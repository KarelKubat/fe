C = $(wildcard *.c)
O = $(patsubst %.c, %.o, $(C))

foo: $(LIB)

install: foo $(LIBDIR)/$(LIB)
$(LIBDIR)/$(LIB): $(LIB)
	mkdir -p $(LIBDIR)
	install $(LIB) $(LIBDIR)

$(LIB): $(O)
	$(CC) $(OPTFLAG) \
	      $(shell ../etc/c-conf -c ../c-conf.cache so-lflags) \
	      -o $(LIB) $(O) $(shell ../etc/c-conf -c ../c-conf.cache lib dl)

%.o: %.c
	$(CC) -Wall -Werror -c \
	      $(shell ../etc/c-conf -c ../c-conf.cache so-cflags) \
	      -DVER=\"$(VER)\" -DLIB=\"$(LIB)\" \
	      -DMAGIC=\"$(MAGIC)\" -DLIBDIR=\"$(LIBDIR)\" \
	      -DBINDIR=\"$(BINDIR)\" $(OPTFLAG) $<

skein.o: skein.c
	$(CC) -c $<

feseedserialize.o: feseedserialize.c magic.h
magic.h: ../Makefile.local
	echo "$(MAGIC)" | perl ../etc/magic.pl > magic.h

clean:
	rm -f $(O) $(LIB) magic.h
