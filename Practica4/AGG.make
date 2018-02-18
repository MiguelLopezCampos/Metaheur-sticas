AGG: ./include/timer.h ./include/random_ppio.h ./src/AGG.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/AGG ./src/AGG.cpp ./src/random_ppio.c ./src/timer.c -I ./include
