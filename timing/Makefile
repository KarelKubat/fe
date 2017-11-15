C = $(wildcard *.c)
T = $(patsubst %.c, %.t, $(C))

foo: $(T)
	@for t in $(T); do \
	  echo; \
	  echo "Running timing: $$t ..."; \
	  $$t || exit 1; \
	done

%.t: %.c ../lib/$(LIB) timing.h
	$(CC) -Wall -Werror -DVER=\"$(VER)\" \
	      $(OPTFLAG) -o $@ $< \
	      -L../main -llocal -L$(LIBDIR) -l$(FE) \
	      $(shell ../etc/c-conf -c ../c-conf.cache lib dl)

clean:
	rm -f $(T)
