AGE: ./include/timer.h ./include/random_ppio.h ./src/AGE.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/AGE ./src/AGE.cpp ./src/random_ppio.c ./src/timer.c -I ./include
