CC:=clang
CFLAGS:=-Wall -Wextra -g $(shell pkg-config --cflags freetype2)
LDFLAGS:=$(shell pkg-config --libs x11 xft freetype2)
APP:=jmenu

all:$(APP)
$(APP):jmenu.o x11.o util.o
	$(CC) -o $@ $^ $(LDFLAGS)
jmenu.o:jmenu.c
	$(CC) -c $<    $(CFLAGS) 
x11.o:x11.c
	$(CC) -c $<    $(CFLAGS)
util.o:util.c
	$(CC) -c $<    $(CFLAGS)
clean:
	rm *.o jmenu
.PHONY: clean all
