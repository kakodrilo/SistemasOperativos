
ejecutar: UNO
	./UNO

UNO: UNO.o
	gcc UNO.o -o UNO

UNO.o: UNO.c
	gcc -c UNO.c -Wall

clear:
	rm -r UNO \
	UNO.o jugador1 jugador2 jugador3 jugador4 mazo ultimaCarta
