ILS: ./include/timer.h ./include/random_ppio.h ./src/ILS.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/ILS ./src/ILS.cpp ./src/random_ppio.c ./src/timer.c -I ./include
