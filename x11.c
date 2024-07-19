#include "x11.h"
#include <X11/Xft/Xft.h>
#include <assert.h>

typedef struct Key {
	struct text text;
	KeySym keysym;
} Key;

static Display* con;
static int      screen;
static Window   root;
static Window   window;
static XEvent   current_event;
static Key      key = {0};
static XftFont* font;
static XftDraw* draw;
static XftColor fg_color;
static XColor   bg_color;

int x11_connect()
{
	con = XOpenDisplay(NULL);
	if (con == NULL) {
		return -1;
	}
	screen = DefaultScreen(con);
	root = DefaultRootWindow(con);
	return 0;
}

int open_window(int x, int y, int w, int h, int bw)
{
	XSetWindowAttributes xwa = {
		.background_pixel  = bg_color.pixel,
		.event_mask        = KeyPressMask,
		.override_redirect = 1,
	};

	if (w <= 0) w = DisplayWidth(con, screen);
	if (h <= 0) h = DisplayHeight(con, screen);
	
	window = XCreateWindow(
		con,
		root,
		x,
		y,
		w,
		h,
		bw,
		DefaultDepth(con,screen),
		InputOutput,
		DefaultVisual(con,screen),
		CWOverrideRedirect | CWBackPixel | CWEventMask,
		&xwa
	);

	XMapWindow(con, window);
	XFlush(con);

	return 0;
}

int grab_keyboard()
{
	while (XGrabKeyboard(con, root, 1, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess);
	return 0;
}

int next_event()
{
	if (XNextEvent(con,&current_event)) {
		return -1;
	}

	if (current_event.type == KeyPress) {
		key.text.size = XLookupString(
			&current_event.xkey,
			key.text.buf,
			sizeof(key.text.buf),
			&key.keysym,
			NULL
		);
	}

	return 0;
}

XEvent event()
{
	return current_event;
}

KeySym which_key()
{
	return key.keysym;
}

struct text* key_to_text()
{
	return &key.text;
}

void ungrab_keyboard()
{
	XUngrabKeyboard(con, CurrentTime);
}

int set_font(const char* font_name)
{
	font = XftFontOpenName(con, screen, font_name);
	if (font == NULL) {
		return -1;
	}
	return 0;
}

int font_height()
{
	assert(font != NULL);
	return font->descent+font->ascent;
}

int set_fg_color(const char* color_code)
{
	return XftColorAllocName(
		con,
		DefaultVisual(con,screen),
		DefaultColormap(con,screen),
		color_code,
		&fg_color
	) ? 0 : -1;
}

int set_bg_color(const char* color_code)
{
	if (!XParseColor(con, DefaultColormap(con,screen), color_code, &bg_color)) {
		return -1;
	}
	if (!XAllocColor(con, DefaultColormap(con,screen), &bg_color)) {
		return -1;
	}
	return 0;
}

int clear_window()
{
	XClearWindow(con, window);
	return 0;
}

int create_draw()
{
	draw = XftDrawCreate(con, window, DefaultVisual(con,screen), DefaultColormap(con,screen));
	if (draw == NULL) {
		return -1;
	}
	return 0;
}

int draw_string(const char* string, const size_t len)
{
	XftDrawStringUtf8(
		draw,
		&fg_color,
		font,
		0,
		font->ascent,
		(XftChar8*)string,
		len
	);
	return 0;
}

void free_font()
{
	if (font == NULL) {
		return;
	}
	XftFontClose(con, font);
}

void free_bg_color()
{
	XFreeColors(con, DefaultColormap(con,screen), &bg_color.pixel, 1, 0);
}

void free_fg_color()
{
	XftColorFree(con, DefaultVisual(con,screen), DefaultColormap(con,screen), &fg_color);
}

void free_draw()
{
	if (draw == NULL) {
		return;
	}
	XftDrawDestroy(draw);
}

void close_window()
{
	if (window == None) {
		return;
	}
	XDestroyWindow(con, window);
}

void x11_disconnect()
{
	XCloseDisplay(con);
}
