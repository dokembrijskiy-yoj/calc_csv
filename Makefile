CC := gcc
CFLAGS := -std=c11 -pedantic-errors -Wpedantic -Wall -Wfatal-errors

program := calc_csv
sources := main.c matrix.c csv.c math_ops.c parse.c csv_matrix.c


objects := $(sources:.c=.o)
depfiles := $(sources:.c=.d)

$(program): $(objects)
	$(CC) -o $@ $(objects)

$(objects): %.o: %.c
	$(CC) $(CFLAGS) -MD -c -o $@ $<

clean:
	rm -f -- $(program) $(objects) $(depfiles)
.PHONY: clean

-include $(depfiles)