# Makefile with conditionals

CC = g++
CFLAGS = -Wall
INCLUDE = -Irpi-rgb-led-matrix/include
LIBS = -Lrpi-rgb-led-matrix/lib -lrgbmatrix -lpthread

ifdef DEBUG
    CFLAGS += -g -DDEBUG
else 
	CFLAGS += -O3
endif

math_app: math.o
    $(CC) $(CFLAGS) -o math_app math.o $(LIBS)

math.o: math.c
    $(CC) $(CFLAGS) -c math.c

clean:
    rm -f math_app math.o