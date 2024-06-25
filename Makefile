CC:=clang
MODE:=debug
APP:=jmenu

CFLAGS_common:=-Wall -Wextra $(shell pkg-config --cflags freetype2)
CFLAGS_debug:=-g
CFLAGS_release:=-O2
CFLAGS=$(CFLAGS_$(MODE)) $(CFLAGS_common)

LDFLAGS_common:=$(shell pkg-config --libs x11 xft freetype2)
LDFLAGS_release:=-s
LDFLAGS=$(LDFLAGS_$(MODE)) $(LDFLAGS_common)

all:$(APP)
$(APP):jmenu.o util.o
	$(CC) -o $@ $^ $(LDFLAGS)
jmenu.o:jmenu.c x11.h
	$(CC) -c $< $(CFLAGS)
util.o:util.c
	$(CC) -c $< $(CFLAGS)
clean:
	rm *.o jmenu
.PHONY: clean all
