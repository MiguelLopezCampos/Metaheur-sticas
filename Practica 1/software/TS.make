TS: ./include/timer.h ./include/random_ppio.h ./src/TS.cpp ./src/timer.c ./src/random_ppio.c
	g++ -o ./bin/TS ./src/TS.cpp ./src/random_ppio.c ./src/timer.c -I ./include

