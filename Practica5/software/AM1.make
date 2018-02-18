AM1: ./include/timer.h ./include/random_ppio.h ./src/AM1.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/AM1 ./src/AM1.cpp ./src/random_ppio.c ./src/timer.c -I ./include
