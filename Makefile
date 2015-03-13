BINDIR = /usr/local/bin
LIBDIR = /usr/local/lib

foo:
	make -C lib
	make -C main

install: foo
	LIBDIR=$(LIBDIR) make -C lib  install
	BINDIR=$(BINDIR) make -C main install

bash: foo
	@export DYLD_LIBRARY_PATH=lib; \
	 export FE_KEY=bla; \
	 echo "Starting bash with new library path and FE_KEY 'bla'"; \
	 echo "Hit ^D to stop."; \
	 /bin/bash; \
	 echo "Back in make environment."

clean:
	make -C lib clean
	make -C main clean
