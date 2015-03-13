C = $(wildcard *.c)
O = $(patsubst %.c, %.o, $(C))
LIB = libfe.dylib

foo: $(LIB)

install: foo
	mkdir -p $(LIBDIR)
	install $(LIB) $(LIBDIR)

$(LIB): $(O)
	$(CC) -g -dynamiclib -Wl,-single_module -o $(LIB) $(O)

%.o: %.c
	$(CC) -Wall -O2 -c -fPIC $<

clean:
	rm -f $(O) $(LIB)
