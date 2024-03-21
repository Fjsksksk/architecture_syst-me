planificateur.exe: planificateur.o
	gcc -o planificateur.exe planificateur.o

planificateur.o: planificateur.c
	gcc -c planificateur.c

clean:
	rm -f planificateur.o planificateur.exe