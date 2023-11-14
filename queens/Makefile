# General flags we'll always use (-Werror to prevent code generation if *any* errors)
GENFLAGS := -Wall -Wextra -pedantic -std=c99 -Wvla -Wfloat-equal -Werror

# -O3 means optimise compiled code for execution speed
OPTIM := $(GENFLAGS) -O3

# Creates a much slower executable but filled with run-time error/bounds checking etc.
SANITIZE := $(GENFLAGS) -g3 -fsanitize=address -fsanitize=undefined

# In the labs you'll probably use clang, but make sure to try gcc too
CC := clang

## THESE FLAGS ARE FOR SDL AND THE EXTENSION CODE.
# ------------------------------
# Optimise code, but allow no warnings during compilation
# pkg-config is accesible after you install SDL2 - on my machine it's equaivalent to the flags : -I/usr/include/SDL2 -D_REENTRANT
PRODFLAGS=$(BASEFLAGS) -O1 -Werror `pkg-config sdl2 --cflags`

# Not optimisation, add in Sanitizer code -> huge executable file but will tell you if an array goes out-of-bounds etc.
DEVFLAGS=$(BASEFLAGS) -fsanitize=address -fsanitize=undefined -g3 
LDLIBS= -lm

# Library linking : maths and SDL2
# pkg-config is accesible after you install SDL2 - on my machine it's equaivalent to the flags : -lSDL2
LIBS =  -lm `pkg-config sdl2 --libs`

# -------------------------------

8q: 8q.c 8q.h 8q_dr.c
	$(CC) 8q.c 8q_dr.c $(OPTIM) -o 8q

8q_san: 8q.c 8q.h
	$(CC) 8q.c 8q_dr.c $(SANITIZE) -o 8q_san

extension: extension.c extension.h neillsdl2.c neillsdl2.h 8q.c 8q.h
	$(CC) extension.c neillsdl2.c 8q.c -o extension  $(PRODFLAGS) $(LIBS)

ext_run:
	./extension -visualize 6

all: 8q 8q_san extension

run:
	./8q 4

san_run:
	./8q_san -verbose 4

clean:
	rm -f 8q 8q_san extension 8q.zip

# Crete a .zip file using ALL files in this directory
zip: 8q.c 8q.h extension.txt extension.c Makefile extension.h neillsdl2.c neillsdl2.h
	zip -r 8q.zip 8q.c 8q.h extension.txt extension.c Makefile extension.h neillsdl2.c neillsdl2.h
