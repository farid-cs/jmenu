#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libjust.h"
#include "config.h"

static window_t window;
static int init();
static int run();
static int clean();

int main()
{
	if (init()) {
		return 1;
	}
	if (run()) {
		return 1;
	}
	if (clean()) {
		return 1;
	}
	return 0;
}

int init()
{
	if (jopen_window(&window, X_COORD, Y_COORD, WIDTH, HEIGHT, BORDER_WIDTH)) {
		fputs("Can't open window", stderr);
		return -1;
	}

	if (!jgrab_keyboard(&window)) {
		puts("Trying to grab keyboard.");
	}

	puts("Keyboard grabbed");

	return 0;
}

int run()
{
	KeySym key;
	while (jnext_key_event(&window, &key) && key != XK_Escape) {
		switch (key) {
		default:
		}
	}

	return 0;
}

int clean()
{
	jungrab_keyboard(&window);
	jclose_window(&window);
	return 0;
}
