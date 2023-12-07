CC=clang #test clang before submission.
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

all: driverbsa_s driverbsa fibmemo sieve_s isfactorial extfibmemo_s driverbsa_v fibmemo_v isfactorial_v sieve_v extfibmemo_v extisfactorial_v extsieve_v

driverbsa_s: bsa.h Alloc/specific.h Alloc/alloc.c driver.c
	$(CC) driver.c Alloc/alloc.c -o driverbsa_s -I./Alloc $(SANITIZE)

driverbsa: bsa.h Alloc/specific.h Alloc/alloc.c driver.c
	$(CC) driver.c Alloc/alloc.c -o driverbsa -I./Alloc $(PRODUCTION)

fibmemo: bsa.h Alloc/specific.h Alloc/alloc.c fibmemo.c
	$(CC) fibmemo.c Alloc/alloc.c -o fibmemo -I./Alloc $(PRODUCTION)

isfactorial: bsa.h Alloc/specific.h Alloc/alloc.c isfactorial.c
	$(CC) isfactorial.c Alloc/alloc.c -o isfactorial -I./Alloc $(PRODUCTION)

sieve_s: bsa.h Alloc/specific.h Alloc/alloc.c sieve.c
	$(CC) sieve.c Alloc/alloc.c -o sieve_s -I./Alloc $(PRODUCTION)

#valgrind versions for speed checks
driverbsa_v: bsa.h Alloc/specific.h Alloc/alloc.c driver.c
	$(CC) driver.c Alloc/alloc.c -o driverbsa_v -I./Alloc $(VALGRIND)

fibmemo_v: bsa.h Alloc/specific.h Alloc/alloc.c fibmemo.c
	$(CC) fibmemo.c Alloc/alloc.c -o fibmemo_v -I./Alloc $(VALGRIND)

isfactorial_v: bsa.h Alloc/specific.h Alloc/alloc.c isfactorial.c
	$(CC) isfactorial.c Alloc/alloc.c -o isfactorial_v -I./Alloc $(VALGRIND)

sieve_v: bsa.h Alloc/specific.h Alloc/alloc.c sieve.c
	$(CC) sieve.c Alloc/alloc.c -o sieve_v -I./Alloc $(VALGRIND)

# profiler
driverbsa_p: bsa.h Alloc/specific.h Alloc/alloc.c driver.c
	$(CC) driver.c Alloc/alloc.c -o driverbsa_p -I./Alloc -pg $(PRODUCTION)

isfactorial_p: bsa.h Alloc/specific.h Alloc/alloc.c isfactorial.c
	$(CC) isfactorial.c Alloc/alloc.c -o isfactorial_p -I./Alloc -pg $(PRODUCTION)


## Only if you do an extension. Uncomment in all, extfibmemo_s and run.
extfibmemo: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) fibmemo.c Extension/extension.c -o extfibmemo -I./Extension $(PRODUCTION)

extfibmemo_s: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) fibmemo.c Extension/extension.c -o extfibmemo_s -I./Extension $(SANITIZE)

extisfactorial: bsa.h Extension/specific.h Extension/extension.c isfactorial.c
	$(CC) isfactorial.c Extension/extension.c -o extisfactorial -I./Extension $(PRODUCTION)

extsieve: bsa.h Extension/specific.h Extension/extension.c sieve.c
	$(CC) sieve.c Extension/extension.c -o extsieve -I./Extension $(PRODUCTION)

extfibmemo_v: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) fibmemo.c Extension/extension.c -o extfibmemo_v -I./Extension $(VALGRIND)

extisfactorial_v: bsa.h Extension/specific.h Extension/extension.c isfactorial.c
	$(CC) isfactorial.c Extension/extension.c -o extisfactorial_v -I./Extension $(VALGRIND)

extsieve_v: bsa.h Extension/specific.h Extension/extension.c sieve.c
	$(CC) sieve.c Extension/extension.c -o extsieve_v -I./Extension $(VALGRIND)

#testing stuff
add: bsa.h Alloc/specific.h Alloc/alloc.c add.c
	$(CC) add.c Alloc/alloc.c -o add -I./Alloc 
	$(CC) add.c Alloc/alloc.c -o add_v -I./Alloc $(DEBUG)
	$(CC) add.c Extension/extension.c -o add_ext -I./Extension
	$(CC) add.c Extension/extension.c -o add_ext_v -I./Extension $(DEBUG)


run: driverbsa driverbsa_s fibmemo sieve_s isfactorial extfibmemo extfibmemo_s extisfactorial extsieve driverbsa_v fibmemo_v isfactorial_v sieve_v extfibmemo_v extisfactorial_v extsieve_v add
	./driverbsa_s
	./isfactorial
	./fibmemo
	./sieve_s
	./extfibmemo
	./extfibmemo_s
	./extisfactorial
	./extsieve
	valgrind --leak-check=full  ./driverbsa_v
	valgrind --leak-check=full  ./isfactorial_v
	valgrind --leak-check=full  ./fibmemo_v
	valgrind --leak-check=full  ./sieve_v
	valgrind --leak-check=full  ./extfibmemo_v
	valgrind --leak-check=full  ./extisfactorial_v
	valgrind --leak-check=full  ./extsieve_v

clean:
	rm -f driverbsa_s driverbsa driverbsa_v extfibmemo fibmemo extfibmemo_s isfactorial sieve_s extfibmemo_v extisfactorial_v extsieve_v isfactorial_v sieve_v fibmemo_v massif.* isfactorial_p driverbsa_p gmon.out add add_ext bsa.zip add_ext add_ext_v add_v
