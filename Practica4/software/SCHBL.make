SCHBL: ./include/timer.h ./include/random_ppio.h ./src/SCHBL.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/SCHBL ./src/timer.c ./src/random_ppio.c ./src/SCHBL.cpp -I ./include
