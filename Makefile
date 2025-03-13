all: del_exe cpu.o build clean

build:
	g++ -std=c++11 -fpermissive cpu.o main.cpp -o main

cpu.o:
	g++ -std=c++11 -fpermissive -c lib/cpu.cpp

del_exe:
	rm -f main

clean:
	rm -f *.o
