ssu_sindex : find.o help.o main.o
	gcc -o ssu_sindex find.o help.o main.o

find.o : find.c
	gcc -c -o find.o find.c

help.o : help.c
	gcc -c -o help.o help.c

main.o : main.c
	gcc -c -o main.o main.c

clean : 
	rm *.o 
