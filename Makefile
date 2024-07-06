include config.mk
APP=jmenu
SRC=jmenu.c x11.c line_buf.c
OBJ=$(SRC:.c=.o)

$(APP):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ):util.h config.h config.mk
%.o:%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -rf *.o jmenu

.PHONY: clean all
