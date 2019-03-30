CC = gcc 
CFLAGS = -g

pipesim: main.o fu.o pipeline.o output.o
	 $(CC) $(CFLAFS) -o pipesim main.o fu.o pipeline.o output.o

asm: asm.o
	$(CC) $(CFLAGS) -o asm asm.c

main.o: main.c fu.h pipeline.h output.h

fu.o: fu.c fu.h pipeline.h

pipeline.o: pipeline.c fu.h pipeline.h

output.o: output.c fu.h pipeline.h output.h

asm.o: asm.c

clean:
	rm -f pipesim main.o fu.o pipeline.o output.o asm asm.o
