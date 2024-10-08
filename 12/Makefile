CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

## EXTENSION FLAGS:
BASEFLAGS=-Wall -Wextra -Wpedantic -std=c99 -Wvla -Wfloat-equal
PRODFLAGS=$(BASEFLAGS) -O1 -Werror `pkg-config sdl2 --cflags`
DEVFLAGS=$(BASEFLAGS) -fsanitize=address -fsanitize=undefined -g3 
LDLIBS= -lm
LIBS =  -lm `pkg-config sdl2 --libs`



all: parse_s interp_s parse_v parse interp extension

parse_s: Parse/parse.h Parse/parse.c
	$(CC) Parse/parse.c General/general.c -o parse_s -I./Parse $(SANITIZE) -lm

parse: Parse/parse.h Parse/parse.c
	$(CC) Parse/parse.c General/general.c -o parse -I./Parse $(PRODUCTION) -lm

parse_v: Parse/parse.h Parse/parse.c
	$(CC) Parse/parse.c General/general.c -o parse_v -I./Parse $(VALGRIND) -lm

run_parse_v: parse_v
			valgrind --leak-check=full ./parse_v TTLs/octagon2.ttl

run_interp_v: interp_v
			valgrind --leak-check=full ./interp_v TTLs/octagon2.ttl out_octagon2s.txt

interp: Interp/interp.h Interp/interp.c neillsimplescreen.c neillsimplescreen.h
	$(CC) neillsimplescreen.c Interp/interp.c General/general.c Interp/Stack/realloc.c -o interp -I./Interp $(PRODUCTION) -lm

interp_s: Interp/interp.h Interp/interp.c neillsimplescreen.c neillsimplescreen.h
	$(CC) neillsimplescreen.c Interp/interp.c General/general.c Interp/Stack/realloc.c -o interp_s -I./Interp $(SANITIZE) -lm

interp_v: Interp/interp.h Interp/interp.c neillsimplescreen.c neillsimplescreen.h Interp/Stack/realloc.c
	$(CC) neillsimplescreen.c Interp/interp.c General/general.c Interp/Stack/realloc.c -o interp_v -I./Interp $(VALGRIND) -lm

extension: Extension/extension.h Extension/extension.c Extension/nsdl/neillsdl2.c Extension/nsdl/neillsdl2.h interp
	$(CC) Extension/nsdl/neillsdl2.c Extension/extension.c -o extension -I./Extension $(PRODFLAGS) $(LIBS)

ext_val: Extension/extension.h Extension/extension.c Extension/nsdl/neillsdl2.c
	$(CC) Extension/nsdl/neillsdl2.c Extension/extension.c -o extension_v -I./Extension $(PRODFLAGS) $(VALGRIND) $(LIBS)

ext_run: extension interp
		./extension

zip:
	zip -r turtle.zip .

ext_run_val: ext_val
		valgrind --leak-check=full ./extension_v

clean:
	rm -f parse_s interp_s interp_v extension extension_s extension_v parse_v out_*.txt out_*.ps out_*.pdf out.pdf out.ps out.txt *.zip interp parse downarrow.pdf downarrow.ps downarrow.txt hypno.txt weirdnums.txt 5x5.txt

test: clean all 
	./Testing/test.sh