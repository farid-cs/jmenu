CC:=clang
CFLAGS:=-Wall -Wextra -lX11

all:jmenu
jmenu:jmenu.o libjust.o
	$(CC) -o $@ $^ $(CFLAGS)
jmenu.o:jmenu.c
	$(CC) -g -c $<
libjust.o:libjust.c
	$(CC) -g -c $<
clean:
	rm *.o jmenu
.PHONY: clean all
