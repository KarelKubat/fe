C = $(wildcard *.c)
T = $(patsubst %.c, %.t, $(C))

foo: $(T)
	@for t in *.t ; do \
	  echo; \
	  echo "Running test: $$t ... "; \
	  $$t || exit 1; \
	done; \
	echo; \
	echo "All tests completed successfully."

%.t: %.c
	$(CC) -Wall -Werror -DMAGIC=\"dontcare\" -o $@ $< -l$(FE)

clean:
	rm -f $(T)

