all: libpluslogic pluslogicint

libpluslogic: pluslogic.o parser.o boolean.o lukasiewicz_ternary.o logicmode.o
	ar rcs libpluslogic.a pluslogic.o parser.o boolean.o lukasiewicz_ternary.o logicmode.o

pluslogicint: libpluslogic pluslogicint.cpp truthtable.cpp
	g++ -std=c++0x -g -o pluslogicint pluslogicint.cpp truthtable.cpp libpluslogic.a -lreadline

pluslogic.o: pluslogic.cpp
	g++ -std=c++0x -g -c pluslogic.cpp

parser.o: parser.cpp
	g++ -std=c++0x -g -c parser.cpp

boolean.o: boolean.cpp
	g++ -std=c++0x -g -c boolean.cpp

lukasiewicz_ternary.o: lukasiewicz_ternary.cpp
	g++ -std=c++0x -g -c lukasiewicz_ternary.cpp

logicmode.o: logicmode.cpp
	g++ -std=c++0x -g -c logicmode.cpp

install:
	cp pluslogicint /usr/bin
	cp libpluslogic.a /usr/lib
	cp pluslogic.h /usr/include

uninstall:
	rm /usr/bin/pluslogicint
	rm /usr/lib/libpluslogic.a
	rm /usr/include/pluslogic.h

clean:
	rm *.o
	rm *.a
	rm pluslogicint
