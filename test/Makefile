C = $(wildcard *.c)
T = $(patsubst %.c, %.t, $(C))

foo: $(T)
	@for t in *.t ; do \
	  echo "Running test: $$t ... "; \
	  $$t; \
	done

%.t: %.c
	$(CC) -Wall -Werror -o $@ $< -l$(FE)

clean:
	rm -f $(T)

