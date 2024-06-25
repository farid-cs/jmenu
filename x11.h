#ifndef X11_H
#define X11_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>

typedef struct {
	unsigned int  key;
	unsigned long state;
} key_event_t;

Display* con;
Window   window;
GC       gc;
Window   root;
XftFont* font;
int      screen;
XftDraw* draw;

static int  connect();
static void disconnect();
static int  open_window(const int, const int, const int, const int, const int);
static void close_window();
static int  grab_keyboard();
static void ungrab_keyboard();
static int  set_font(const char*);
static int  get_font_size();
static void free_font();
static int  draw_string(const char* string);
static int  next_key_event(key_event_t*);

int connect()
{
	con = XOpenDisplay(NULL);
	if (con == NULL) {
		return -1;
	}
	screen  = DefaultScreen(con);
	return 0;
}

int open_window(const int x, const int y, const int w, const int h, const int bw)
{
	root = RootWindow(con,screen);

	XSetWindowAttributes xwa = {
		.background_pixel  = BlackPixel(con,window),
		.border_pixel      = WhitePixel(con,window),
		.event_mask        = KeyPressMask,
		.override_redirect = 1,
	};

	unsigned int width  = (w > 0) ? w : DisplayWidth(con,window);
	unsigned int height = (h > 0) ? h : DisplayHeight(con,window);

	window = XCreateWindow(
			con,
			root,
			x,
			y,
			width,
			height,
			bw,
			DefaultDepth(con,screen),
			InputOutput,
			DefaultVisual(con,screen), 
			CWOverrideRedirect | CWBorderPixel | CWBackPixel | CWEventMask,
			&xwa
	);

	XMapWindow(con,window);

	gc = XCreateGC(con,window, 0, NULL);

	XFlush(con);

	return 0;
}

int grab_keyboard()
{
	while (XGrabKeyboard(con,window,1,GrabModeAsync,GrabModeAsync,CurrentTime) != GrabSuccess);
	return 0;
}

int next_key_event(key_event_t* key_event)
{
	XEvent ev;

	while (!XNextEvent(con,&ev)) {
		if (ev.type != KeyPress) {
			break;
		}
	}

	key_event->key = XKeycodeToKeysym(con,ev.xkey.keycode,0);
	key_event->state = ev.xkey.state;
	return 0;
}

void ungrab_keyboard()
{
	XUngrabKeyboard(con,CurrentTime);
}

int set_font(const char* font_name)
{
	font = XftFontOpenName(con,screen,font_name);
	if (font == NULL) {
		return -1;
	}
	return 0;
}

int get_font_size()
{
	return font->descent+font->ascent;
}

int draw_string(const char* string)
{
	XftColor color = {0};

	if (!XftColorAllocName(con,
			       DefaultVisual(con,screen),
			       DefaultColormap(con,screen),
			       "#bbbbbb",
			       &color)
	)
	{
		return -1;
	}

	draw = XftDrawCreate(con,window,DefaultVisual(con,screen),None);

	if (draw == NULL) {
		return -1;
	}

	XftDrawStringUtf8(
		draw,
		&color,
	        font,
		0,
		font->ascent,
		(XftChar8*)string,
		strlen(string)
	);

	XftDrawDestroy(draw);

	return 0;
}

void free_font()
{
	XftFontClose(con,font);
}

void free_draw()
{
	XftDrawDestroy(draw);
}

void close_window()
{
	XFreeGC(con,gc);
	XDestroyWindow(con,window);
}

void disconnect()
{
	XCloseDisplay(con);
}
#endif
