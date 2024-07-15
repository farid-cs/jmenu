include config.mk
APP=jmenu
SRC=jmenu.c x11.c line_buf.c util.c
OBJ=$(SRC:.c=.o)

build:$(APP)

$(APP):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

test:$(APP)
	echo Some text | ./$(APP)

$(OBJ):util.h x11.h config.h config.mk
%.o:%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -rf *.o jmenu

.PHONY: clean build
