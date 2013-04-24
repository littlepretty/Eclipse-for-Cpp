main: main.o
	  g++ -o main main.o -L C:/MinGW/lib/gcc/mingw32/4.5.2 -lstdc++
main.o: main.cpp
	  g++ -ggdb -c main.cpp
all:
		${MAKE} main
clean: 
		-del main.o