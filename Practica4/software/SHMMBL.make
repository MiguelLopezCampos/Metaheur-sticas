SHMMBL: ./include/timer.h ./include/random_ppio.h ./src/SHMMBL.cpp ./src/timer.c ./src/random_ppio.c
	g++ -O3 -o ./bin/SHMMBL ./src/timer.c ./src/random_ppio.c ./src/SHMMBL.cpp -I ./include
