#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "config.h"
#include "util.h"
#include "x11.h"

static struct line_buf input_line = {0};
static char *input;

static char *
read_stdin(const int buf_size)
{
	char *buf = calloc(1, buf_size);
	if (buf == NULL) {
		return NULL;
	}

	fread(buf, 1, buf_size, stdin);

	if (ferror(stdin)) {
		return NULL;
	}

	return buf;
}

static int
init()
{
	input = read_stdin(3000);
	if (!input) {
		eputs("Can't read from stdin");
		return -1;
	}
	if (setlocale(LC_CTYPE, "") == NULL) {
		eputs("Can't set default locale");
		return -1;
	}
	if (x11_connect()) {
		eputs("Can't connect to X11 server");
		return -1;
	}
	if (set_fg_color(foreground_color)) {
		eputs("Can't set foreground color");
		return -1;
	}
	if (set_bg_color(background_color)) {
		eputs("Can't set background color");
		return -1;
	}
	if (set_font(font)) {
		eputs("Can't set font");
		return -1;
	}
	if (open_window(pos_x, pos_y, window_width, font_height(), border_width)) {
		eputs("Can't open the window");
		return -1;
	}
	if (grab_keyboard()) {
		eputs("Can't grab the keyboard");
		return -1;
	}
	if (create_draw()) {
		eputs("Can't create a draw");
		return -1;
	}
	return 0;
}

static int
keypress()
{
	switch (which_key()) {
	case XK_Shift_L:
	case XK_Shift_R:
	case XK_Control_L:
	case XK_Control_R:
	case XK_Meta_L:
	case XK_Meta_R:
		break;
	case XK_BackSpace:
		backspace(&input_line);
		break;
	case XK_Return: puts(input_line.buf);
		return 1;
	case XK_Escape:
		return -1;
	default:
		append_text(&input_line, key_to_text());
		break;
	}

	return 0;
}

static int
handle_event()
{
	switch (event().type) {
	case KeyPress:
		return keypress();
	default:
		break;
	}
	return 0;
}

static int
draw_menu()
{
	if (clear_window()) {
		eputs("Can't clear the window");
		return -1;
	}
	if (draw_string(input_line.buf, input_line.pos)) {
		eputs("Can't print on the window");
		return -1;
	}
	return 0;
}

static int
run()
{
	while (!next_event()) {
		switch (handle_event()) {
		case 1:
			return 0;
		case -1:
			return -1;
		default:
			break;
		}
		if (draw_menu()) {
			break;
		}
	}
	return -1;
}

static
void clean()
{
	free_draw();
	ungrab_keyboard();
	close_window();
	free_font();
	free_bg_color();
	free_fg_color();
	x11_disconnect();
	free(input);
}

int
main()
{
	if (init()) {
		goto ERROR;
	}
	if (run()) {
		goto ERROR;
	}
	clean();
	return 0;
ERROR:
	clean();
	return 1;
}
