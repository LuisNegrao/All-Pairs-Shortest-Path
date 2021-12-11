CC=mpicc
ARGS=-Wall
LINKS=-lm
OUTPUT=fox

all:
	$(CC) main.c matrixutil.c fox.c -o $(OUTPUT) $(LINKS)

clean:
	rm -f *.o $(OUTPUT)