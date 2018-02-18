SA: ./include/timer.h ./include/random_ppio.h ./src/SA.cpp ./src/timer.c ./src/random_ppio.c
	g++ -o ./bin/SA ./src/SA.cpp ./src/random_ppio.c ./src/timer.c -I ./include
