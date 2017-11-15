C = $(wildcard *.c libc/*.c)
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
	      -DHAVE_LIBIO=$(shell ../etc/c-conf -c ../c-conf.cache \
			     ifheader libio.h HAVE_LIBIO_H_) \
	      -DLIBDIR=\"$(LIBDIR)\" \
	      -DBINDIR=\"$(BINDIR)\" $(OPTFLAG) -o $@ $<

skein.o: skein.c
	$(CC) -c $(shell ../etc/c-conf -c ../c-conf.cache so-cflags) $<

feseedserialize.o: feseedserialize.c

clean:
	rm -f $(O) $(LIB)
