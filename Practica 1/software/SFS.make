SFS: ./include/timer.h ./include/random_ppio.h ./src/SFS.cpp ./src/timer.c ./src/random_ppio.c
	g++ -o ./bin/SFS ./src/timer.c ./src/random_ppio.c ./src/SFS.cpp -I ./include
