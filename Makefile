CC:=clang
CFLAGS:=-Wall -Wextra -g
LDFLAGS:=-lX11
APP:=jmenu

all:$(APP)
$(APP):jmenu.o libjust.o
	$(CC) -o $@ $^ $(LDFLAGS)
jmenu.o:jmenu.c
	$(CC) -c $< $(CFLAGS) 
libjust.o:libjust.c
	$(CC) -c $< $(CFLAGS)
clean:
	rm *.o jmenu
.PHONY: clean all
