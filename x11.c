/* Just a small abstraction layer over what libX11 and libXft provide */
#ifndef X11_C
#define X11_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "x11.h"

int open_window(window_t* window, const int x, const int y, const int w, const int h, const int bw)
{
	window->dp = XOpenDisplay(NULL);
	if (window->dp == NULL) {
		return -1;
	}

	window->scr  = DefaultScreen(window->dp);
	window->root = RootWindow(window->dp, window->scr);

	XSetWindowAttributes xwa = {
		.background_pixel  = BlackPixel(window->dp, window->scr),
		.border_pixel      = WhitePixel(window->dp, window->scr),
		.event_mask        = KeyPressMask,
		.override_redirect = 1,
	};

	unsigned int width  = (w > 0) ? w : DisplayWidth(window->dp, window->scr);
	unsigned int height = (h > 0) ? h : DisplayHeight(window->dp, window->scr);

	window->id = XCreateWindow(
			window->dp,
			window->root,
			x,
			y,
			width,
			height,
			bw,
			DefaultDepth(window->dp, window->scr),
			InputOutput,
			DefaultVisual(window->dp, window->scr), 
			CWOverrideRedirect | CWBorderPixel | CWBackPixel | CWEventMask,
			&xwa
	);

	XMapWindow(window->dp, window->id);

	window->gc = XCreateGC(window->dp, window->id, 0, NULL);

	XFlush(window->dp);

	return 0;
}

int grab_keyboard(window_t* window)
{
	while (XGrabKeyboard(window->dp, window->id, 1, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess);
	return 0;
}

int next_key_event(window_t* window, key_event_t* key_event)
{
	XEvent ev;

	while (!XNextEvent(window->dp, &ev)) {
		if (ev.type != KeyPress) {
			continue;
		}

		key_event->key = XKeycodeToKeysym(window->dp, ev.xkey.keycode, 0);
		key_event->state = ev.xkey.state;
		break;
	}

	return 0;
}

void ungrab_keyboard(window_t* window)
{
	XUngrabKeyboard(window->dp, CurrentTime);
}

int set_font(window_t* window, const char* font_name)
{
	window->font = XftFontOpenName(window->dp, window->scr, font_name);
	if (window->font == NULL) {
		return -1;
	}
	return 0;
}

int draw_string(window_t* window, const char* string)
{
	XftColor color = {0};

	if (!XftColorAllocName(window->dp,
			       DefaultVisual(window->dp, window->scr),
			       DefaultColormap(window->dp, window->scr),
			       "#ffffff",
			       &color)
	)
	{
		fputs("Can't create color", stderr);
		return -1;
	}

	XftDraw *draw = XftDrawCreate(window->dp, window->id, DefaultVisual(window->dp, window->scr), None);

	if (draw == NULL) {
		fputs("Can't create draw", stderr);
		return -1;
	}

	XftDrawStringUtf8(
		draw,
		&color,
	        window->font,
		10,
		18,
		(XftChar8*)string,
		strlen(string)
	);

	return 0;
}

void free_font(window_t* window)
{
	XftFontClose(window->dp, window->font);
}

void close_window(window_t* window)
{
	XFreeGC(window->dp, window->gc);
	XDestroyWindow(window->dp,window->id);
	XCloseDisplay(window->dp);
}
#endif
