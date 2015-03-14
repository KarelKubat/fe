C = $(wildcard *.c)
O = $(patsubst %.c, %.o, $(C))

foo: $(LIB)

install: foo $(LIBDIR)/$(LIB)
$(LIBDIR)/$(LIB): $(LIB)
	mkdir -p $(LIBDIR)
	install $(LIB) $(LIBDIR)

$(LIB): $(O)
	$(CC) -g -dynamiclib -Wl,-single_module -o $(LIB) $(O)

%.o: %.c
	$(CC) -Wall -Werror -O2 -c -fPIC -DVER=\"$(VER)\" -DLIB=\"$(LIB)\" \
	      -DLIBDIR=\"$(LIBDIR)\" -DBINDIR=\"$(BINDIR)\" $<

clean:
	rm -f $(O) $(LIB)
