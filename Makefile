bleep: parser.o node.o game.o object.o star.o maths.o kinetics.o sounds.o asteroid.o graphics.o player.o bleep.o
	g++ -g -std=c++17 -o bleep ./game.o ./parser.o ./node.o ./kinetics.o ./asteroid.o ./sounds.o ./player.o ./graphics.o ./maths.o ./object.o ./star.o ./bleep.o -lasound -lpthread -lpng -lGL -lX11

bleep.o: ./src/bleep.cpp
	g++ -g -std=c++17 -c ./src/bleep.cpp -I./src/objects/ -I./src/helper -I./src/thirdParty -I./src/components

parser.o: ./src/parser.cpp ./src/parser.h
	g++ -g -std=c++17 -c ./src/parser.cpp -I./src/helper

node.o: ./src/node.cpp ./src/node.h
	g++ -g -std=c++17 -c ./src/node.cpp -I./src/helper

game.o: ./src/game.cpp ./src/game.h
	g++ -g -std=c++17 -c ./src/game.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

object.o: ./src/objects/object.cpp ./src/objects/object.h
	g++ -g -std=c++17 -c ./src/objects/object.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

star.o: ./src/objects/star.cpp ./src/objects/star.h
	g++ -g -std=c++17 -c ./src/objects/star.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

player.o: ./src/objects/player.cpp ./src/objects/player.h
	g++ -g -std=c++17 -c ./src/objects/player.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

asteroid.o: ./src/objects/asteroid.cpp ./src/objects/asteroid.h
	g++ -g -std=c++17 -c ./src/objects/asteroid.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

maths.o: ./src/helper/maths.cpp ./src/helper/maths.h
	g++ -g -std=c++17 -c ./src/helper/maths.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

kinetics.o: ./src/components/kinetics.cpp ./src/components/kinetics.h
	g++ -g -std=c++17 -c ./src/components/kinetics.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

sounds.o: ./src/components/sounds.cpp ./src/components/sounds.h
	g++ -g -std=c++17 -c ./src/components/sounds.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

graphics.o: ./src/components/graphics.cpp ./src/components/graphics.h
	g++ -g -std=c++17 -c ./src/components/graphics.cpp -I./src/helper -I./src/objects -I./src -I./src/thirdParty

clean:
	rm -f bleep parser.o node.o game.o object.o star.o maths.o kinetics.o sounds.o asteroid.o graphics.o player.o bleep.o

dist-clean: clean
	rm -rf dist

dist: bleep
	mkdir dist
	cp -r graphics settings sounds dist/
	cp bleep dist/
