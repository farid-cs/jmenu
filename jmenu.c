#include <stdio.h>
#include "x11.h"
#include "config.h"
#include "util.h"

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
		clean();
		return 1;
	}
	if (clean()) {
		return 1;
	}
	return 0;
}

int init()
{
	if (open_window(&window, X_COORD, Y_COORD, WIDTH, HEIGHT, BORDER_WIDTH)) {
		eputs("Can't open the window\n");
		return -1;
	}
	if (grab_keyboard(&window)) {
		eputs("Can't grab the keyboard\n");
		return -1;
	}
	if (set_font(&window, FONT)) {
		eputs("Can't set font\n");
		return -1;
	}
	return 0;
}

int run()
{
	key_event_t ev;
	if (draw_string(&window, "Hello how are you here")) {
		eputs("Can't set color\n");
		return -1;
	}
	while (!next_key_event(&window, &ev) && ev.key != XK_Escape) {
		switch (ev.key) {
		default:
			puts("Some key event");
		}
	}
	return 0;
}

int clean()
{
	free_font(&window);
	ungrab_keyboard(&window);
	close_window(&window);
	return 0;
}
