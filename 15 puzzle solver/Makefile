all: ida wida

ida: main1.c idaFunctions.o
	gcc -Wall -O3 main1.c idaFunctions.o -o ida

wida: main2.c widaFunctions.o
	gcc -Wall -O3 main2.c widaFunctions.o -o wida

idaFunctions.o: idaFunctions.c idaFunctions.h
	gcc -Wall -c idaFunctions.c

widaFunctions.o: widaFunctions.c widaFunctions.h
	gcc -Wall -c widaFunctions.c