CC = g++ -g

spellchecker: spellchecker.o
	$(CC) spellchecker.o -o spellchecker

spellchecker.o: spellchecker.cpp
	$(CC) -c spellchecker.cpp

clean:
	rm -f spellchecker spellchecker.o