test: cpu.o main.cpp
	g++ -std=c++11 -fpermissive cpu.o main.cpp -o main

cpu.o:
	g++ -std=c++11 -c lib/cpu.cpp
