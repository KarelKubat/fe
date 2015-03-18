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

%.t: %.c
	$(CC) -Wall -Werror -DMAGIC=\"dontcare\" -o $@ $< \
	      -L../main -llocal -l$(FE)

clean:
	rm -f $(T)

