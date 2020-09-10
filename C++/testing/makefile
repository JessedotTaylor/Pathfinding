Main.exe		: 	Main.o
	g++ -Wl,-s -o Main.exe Main.o -l gdi32 
			
Main.o		:	Main.cpp
	g++ -c -std=c++11 -O2  -fpermissive -fconserve-space -Wno-write-strings  Main.cpp
