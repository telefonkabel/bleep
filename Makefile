bleep: jsParser.o game.o object.o star.o maths.o kinetics.o sound.o asteroid.o hawking.o graphics.o blackHole.o bleep.o
	g++ -g -std=c++17 -o bleep ./game.o ./jsParser.o ./kinetics.o ./asteroid.o ./hawking.o ./sound.o ./blackHole.o ./graphics.o ./maths.o ./object.o ./star.o ./bleep.o -lasound -lpthread -lpng -lGL -lX11

bleep.o: ./src/bleep.cpp
	g++ -g -std=c++17 -c ./src/bleep.cpp -I./src/

jsParser.o: ./src/jsParser.cpp ./src/jsParser.h
	g++ -g -std=c++17 -c ./src/jsParser.cpp -I./src/

game.o: ./src/game.cpp ./src/game.h
	g++ -g -std=c++17 -c ./src/game.cpp -I./src/

object.o: ./src/objects/object.cpp ./src/objects/object.h
	g++ -g -std=c++17 -c ./src/objects/object.cpp -I./src/

star.o: ./src/objects/star.cpp ./src/objects/star.h
	g++ -g -std=c++17 -c ./src/objects/star.cpp -I./src/

blackHole.o: ./src/objects/blackHole.cpp ./src/objects/blackHole.h
	g++ -g -std=c++17 -c ./src/objects/blackHole.cpp -I./src/

asteroid.o: ./src/objects/asteroid.cpp ./src/objects/asteroid.h
	g++ -g -std=c++17 -c ./src/objects/asteroid.cpp -I./src/
	
hawking.o: ./src/objects/hawking.cpp ./src/objects/hawking.h
	g++ -g -std=c++17 -c ./src/objects/hawking.cpp -I./src/

maths.o: ./src/helper/maths.cpp ./src/helper/maths.h
	g++ -g -std=c++17 -c ./src/helper/maths.cpp -I./src/

kinetics.o: ./src/components/kinetics.cpp ./src/components/kinetics.h
	g++ -g -std=c++17 -c ./src/components/kinetics.cpp -I./src/

sound.o: ./src/sound.cpp ./src/sound.h
	g++ -g -std=c++17 -c ./src/sound.cpp -I./src/

graphics.o: ./src/components/graphics.cpp ./src/components/graphics.h
	g++ -g -std=c++17 -c ./src/components/graphics.cpp -I./src/

clean:
	rm -f bleep jsParser.o game.o object.o star.o maths.o kinetics.o sound.o asteroid.o hawking.o graphics.o blackHole.o bleep.o

dist-clean: clean
	rm -rf dist

dist: bleep
	mkdir dist
	cp -r graphics settings sounds dist/
	cp bleep dist/
