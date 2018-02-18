AM2: ./include/timer.h ./include/random_ppio.h ./src/AM2.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/AM2 ./src/AM2.cpp ./src/random_ppio.c ./src/timer.c -I ./include
