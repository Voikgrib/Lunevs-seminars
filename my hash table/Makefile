COMPILER=gcc

all: default

#===============! Usual usage !==================#

default: main.c main.o open_adress_hash.default
	$(COMPILER) -o programm.out main.o open_adress_hash.o

open_adress_hash.default: open_adress_hash.c
	$(COMPILER) open_adress_hash.c -c -o open_adress_hash.o

main.o: main.c
	$(COMPILER) main.c -c -o main.o

#===============! Tests here !==================#

test: clean open_adress_hash.o open_adress_hash.c
	$(COMPILER) test_for_all.c -o test_for_all.out open_adress_hash.o -lgcov -DTEST_BUILD
	valgrind ./test_for_all.out
	gcov open_adress_hash.gcda

open_adress_hash.o: open_adress_hash.c
	$(COMPILER) open_adress_hash.c -c -o open_adress_hash.o -ftest-coverage -fprofile-arcs -g -DTEST_BUILD

#=============! Cosmetic stuff !================#
help:
	cat help.txt

#===============! Cleaners !====================#
clean:
	rm -rf a.out *.o *.d *.gcno *.gcov *.gcda


-include *.d


