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
# [KK 2017-04-30] 1.06 Signal interrupts only installed when directly crypting,
#		       not in pass-thru mode.
# [KK 2017-10-17] 1.07 Linux build fixed. BINDIR and LIBDIR can be set
#		       externally. Tests log to stderr instead of syslog.
# [KK 2017-11-15] 2.00 Algorithm change, files transcrypted with previous
#                      version 1.* cannot be handled by this. No more local
#                      magic setting.
# [KK 2017-11-15] 2.01 Targets are stored as resolved filenames.
# [KK 2017-11-22] 2.02 Fixed signed/unsigned warnings and fe_Hash() calling
# [KK 2017-11-24] 2.03 Fixed dirname/basename handling for MacOSX and the
#		       reference test
# [KK 2018-09-26] 2.04 Cosmetic fix on progress bar during fe -f
# [KK 2018-12-12] 2.05 Added openat() support
VER     = 2.05

# Target paths. You probably don't want to install under private dirs under
# $HOME, it seriously messes up library preloading and you have to modify
# /etc/ld.so.conf and so on, and you need to know what you are doing.
BINDIR  = /usr/local/bin
LIBDIR  = /usr/local/lib

# Internal settings
FE      ?= fe
LIB     ?= $(shell etc/c-conf -c c-conf.cache so-name $(FE))
OPTFLAG ?= $(shell etc/c-conf -c c-conf.cache optflags)

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

foo:
	@cat etc/Makefile.help
	@exit 1

local:
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG="$(OPTFLAG)" make -C lib
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  USYS=$(USYS) OPTFLAG="$(OPTFLAG)" make -C main

profiling:
	OPTFLAG=-pg make install
	@echo
	@echo "Remember to recompile and reinstall without profiling once"
	@echo "you are ready!"
	@echo

timing: local
	LIB=$(LIB) OPTFLAG="$(OPTFLAG)" FE=$(FE) VER=$(VER) make -C timing

test: install
	LIBDIR=$(LIBDIR) LIB=$(LIB) FE=$(FE) OPTFLAG="$(OPTFLAG)" make -C test

install:
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  OPTFLAG="$(OPTFLAG)" make -C lib  install
	BINDIR=$(BINDIR) LIBDIR=$(LIBDIR) LIB=$(LIB) VER=$(VER) FE=$(FE) \
	  USYS=$(USYS) OPTFLAG="$(OPTFLAG)" make -C main install

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
