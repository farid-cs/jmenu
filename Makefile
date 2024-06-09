CC:=clang
CFLAGS:=-Wall -Wextra -g
LDFLAGS:=-lX11

all:jmenu
jmenu:jmenu.o libjust.o
	$(CC) -o $@ $^ $(LDFLAGS)
jmenu.o:jmenu.c
	$(CC) -c $< $(CFLAGS) 
libjust.o:libjust.c
	$(CC) -c $< $(CFLAGS)
clean:
	rm *.o jmenu
.PHONY: clean all
