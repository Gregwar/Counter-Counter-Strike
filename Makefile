all: CS

main.o: main.c
	gcc -IlibGo/ -c main.c -o main.o

CS: main.o libGo/libgo.a
	gcc main.o -lgo -LlibGo/ -o CS

libGo/libgo.a:
	make -C libGo

clean:
	rm -rf CS *.o
