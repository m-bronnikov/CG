all: hello

hello: shader.o main.o
	g++ shader.o main.o -o hello -lGL -lGLEW -lSOIL -lglfw 
	rm -rf *.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

shader.o: shader.cpp
	g++ -std=c++11 -c shader.cpp

clean:
	rm -rf *.o hello
