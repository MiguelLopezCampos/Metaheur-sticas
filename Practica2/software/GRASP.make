GRASP: ./include/timer.h ./include/random_ppio.h ./src/GRASP.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/GRASP ./src/GRASP.cpp ./src/random_ppio.c ./src/timer.c -I ./include
