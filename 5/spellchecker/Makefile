# General flags we'll always use (use -Werror to prevent code generation if *any* errors)
GENFLAGS := -Wall -Wextra -pedantic -std=c99 -Wvla -Wfloat-equal #-Werror

# -O2 means optimise comiled code for execution speed
OPTIM := $(GENFLAGS) -O2

# In the labs you'll proably use clang, but make sure to try gcc too
CC := clang

all: 1

1: 1.c
	$(CC) 1.c $(OPTIM) -o 1

clean:
	rm -f 1

run: all
	./1
