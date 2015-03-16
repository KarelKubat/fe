BINDIR  = /usr/local/bin
LIBDIR  = /usr/local/lib
FE      = fe
LIB     = lib$(FE).dylib
VER     = 1.00
OPTFLAG = -O2

# Versioning:
# [KK 2015-03-13] 1.00, initial

include Makefile.local

foo:
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" make -C lib
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" make -C main

test: install
	LIB=$(LIB) FE=$(FE) OPTFLAG=$(OPTFLAG) make -C test

install:
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" make -C lib  install
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" make -C main install

clean:
	LIB=$(LIB) FE=$(FE) make -C lib clean
	LIB=$(LIB) FE=$(FE) make -C main clean
	LIB=$(LIB) FE=$(FE) make -C test clean
