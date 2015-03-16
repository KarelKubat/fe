C = $(wildcard *.c)
O = $(patsubst %.c, %.o, $(C))

foo: $(LIB)

install: foo $(LIBDIR)/$(LIB)
$(LIBDIR)/$(LIB): $(LIB)
	mkdir -p $(LIBDIR)
	install $(LIB) $(LIBDIR)

$(LIB): $(O)
	$(CC) $(OPTFLAG) -dynamiclib -Wl,-single_module -o $(LIB) $(O)

%.o: %.c
	$(CC) -Wall -Werror -c -fPIC -DVER=\"$(VER)\" -DLIB=\"$(LIB)\" \
	      -DMAGIC=\"$(MAGIC)\" -DLIBDIR=\"$(LIBDIR)\" \
	      -DBINDIR=\"$(BINDIR)\" $(OPTFLAG) $<

clean:
	rm -f $(O) $(LIB)
