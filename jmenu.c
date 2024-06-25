#include <stdio.h>
#include "x11.h"
#include "config.h"
#include "util.h"

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
	if (connect()) {
		eputs("Can't connect to X11 server\n");
		return -1;
	}
	if (set_font(FONT)) {
		eputs("Can't set font\n");
		return -1;
	}
	if (open_window(POS_X, POS_Y, WIDTH, get_font_size(), BORDER_WIDTH)) {
		eputs("Can't open the window\n");
		return -1;
	}
	if (grab_keyboard()) {
		eputs("Can't grab the keyboard\n");
		return -1;
	}
	return 0;
}

int run()
{
	key_event_t ev;
	if (draw_string("Joke")) {
		eputs("Can't print on the window\n");
		return -1;
	}
	while (!next_key_event(&ev) && ev.key != XK_Escape) {
		switch (ev.key) {
		default:
			puts("Some key event");
		}
	}
	return 0;
}

int clean()
{
	free_font();
	ungrab_keyboard();
	close_window();
	disconnect();
	return 0;
}
