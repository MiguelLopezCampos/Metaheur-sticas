3NN: ./include/timer.h ./include/random_ppio.h ./src/3NN.cpp ./src/timer.c ./src/random_ppio.c
	g++ -o ./bin/3NN ./src/3NN.cpp ./src/random_ppio.c ./src/timer.c -I ./include
