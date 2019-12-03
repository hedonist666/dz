cc = gcc
CFLAGS = -g 
DEPS = main.h

mane: main.o 
	gcc main.o $(DEPS) $(CFLAGS)
.PHONY: mane

%.o: %.c %.h
	gcc -c $< -o $@ $(CFLAGS)

clean:
	rm -f ./*.o ./*.gch ./.*.swp a.out
.PHONY: clean
