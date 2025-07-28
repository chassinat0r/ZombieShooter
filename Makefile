default:
	g++ -g -Iinclude -o main src/*.cpp src/glad.c src/engine/*.cpp -lglfw -lGL -Wno-write-strings -Wno-narrowing -Wno-deprecated

run:
	./main
