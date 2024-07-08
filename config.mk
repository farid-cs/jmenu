CC=cc
CFLAGS=-g -O0 -Wall -Wextra $(INC)
LDFLAGS=$(LIB)

LIB=-lX11 -lXft -lfreetype
INC=-I/usr/include/freetype2
