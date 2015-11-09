# Versioning:
# [KK 2015-03-23] 1.00 Initial
# [KK 2015-03-24] 1.01 Flags -i and -v, FE_CTX changed due to -i
# [KK 2015-03-26] 1.02 Implemented shared memory, flag -e
# [KK 2015-03-28] 1.03 Temp buffer files for emacs and vi added. For every
#		       file.txt, targets .#file.txt and .file.txt.swp are used
# [KK 2015-03-30] 1.04 Key is reprompted in interactive mode. Interactively
#		       entered zero-length keys are rejected. Progress meter
#		       added during -f transcryption. Many stop-signals are
#		       now ignored while transcrypting a file. Support for
#		       fprintf() added (though fscanf() is impossible) plus
#		       fgetc(), fgets(), fputc(), fputs() etc. Flag -d added
#		       with ctx.debug setting.
# [KK 2015-11-09] 1.05 Added long options --target etc. Adjusted usage info.
#		       Added a Porting section to TECHINFO.txt. Progress bar
#		       during "fe -f ..." shows the ETA in secs.
VER     = 1.05

# Target paths
BINDIR  = /usr/local/bin
LIBDIR  = /usr/local/lib

# Internal settings
FE      = fe
LIB     = $(shell etc/c-conf -c c-conf.cache so-name $(FE))
OPTFLAG = $(shell etc/c-conf -c c-conf.cache optflags)
# OPTFLAG = -g

# Dist archive
TAR     = /tmp/fe-$(VER).tar.gz

# What system are we on
UNAME   = $(shell uname)
ifeq ($(UNAME), Darwin)
    USYS = UN_DARWIN
endif
ifeq ($(UNAME), Linux)
    USYS = UN_LINUX
endif

include Makefile.local

foo:
	@cat etc/Makefile.help
	@exit 1

local:
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" make -C lib
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  USYS=$(USYS) OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" \
	  make -C main

test: install
	LIB=$(LIB) FE=$(FE) OPTFLAG=$(OPTFLAG) make -C test

install:
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" make -C lib  install
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  USYS=$(USYS) OPTFLAG=$(OPTFLAG) MAGIC="$(MAGIC)" \
	  make -C main install
	@if [ "$(OPTFLAG)" = "-g" ] ; then \
	    echo; \
	    echo "WARNING: Compiled with debugging symbols"; \
	    echo "WARNING: Edit Makefile and recompile for production"; \
	 fi

clean:
	LIB=$(LIB) FE=$(FE) make -C lib clean
	LIB=$(LIB) FE=$(FE) make -C main clean
	LIB=$(LIB) FE=$(FE) make -C test clean
	find . -name \*.bak -type f -exec rm {} \;
	rm -f c-conf.cache

dist: clean
	(cd ..; \
         tar -c -z --exclude fe/Makefile.local \
	     --exclude fe/.git --exclude fe/.gitignore -f $(TAR) fe/)
