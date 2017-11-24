C = $(wildcard *.c)
T = $(patsubst %.c, %.t, $(C))

foo: $(T)
	@for t in *.pl; do \
	  echo; \
	  echo "Running test: $$t ..."; \
	  perl $$t || exit 1; \
	done
	@for t in *.t ; do \
	  echo; \
	  echo "Running test: $$t ..."; \
	  $$t || exit 1; \
	done;
	@echo; \
	echo "All tests completed successfully."

%.t: %.c ../lib/$(LIB)
	$(CC) -Wall -Werror -g -o $@ $< \
	      -L../main -llocal -L$(LIBDIR) -l$(FE) \
	      $(shell ../etc/c-conf -c ../c-conf.cache lib dl)

clean:
	rm -rf $(T) *.t.dSYM

