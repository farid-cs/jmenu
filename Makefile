include config.mk
APP=jmenu
SRC=jmenu.c x11.c line_buf.c util.c
OBJ=$(SRC:.c=.o)

$(APP):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

test:$(APP)
	echo Some text | ./$(APP)

%.o:%.c
	$(CC) -c $< $(CFLAGS)

$(OBJ): config.mk
jmenu.o: jmenu.c util.c x11.c line_buf.c util.h line_buf.h config.h
x11.o: x11.c x11.h
line_buf.o: line_buf.c line_buf.h

clean:
	rm -rf *.o jmenu

.PHONY: clean test
