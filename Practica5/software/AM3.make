AM3: ./include/timer.h ./include/random_ppio.h ./src/AM3.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/AM3 ./src/AM3.cpp ./src/random_ppio.c ./src/timer.c -I ./include
