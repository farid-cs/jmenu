#ifndef XSERVER_C
#define XSERVER_C

#include "libjust.h"
#include <stdio.h>
#include <stdlib.h>

int jopen_window(window_t* window, const int x, const int y, const int w, const int h, const int bw)
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
		.event_mask        = KeyPressMask | KeyReleaseMask,
		.override_redirect = 1,
	};

	unsigned int width  = (w > 0) ? w : DisplayWidth(window->dp, window->scr);
	unsigned int height = (h > 0) ? h : DisplayHeight(window->dp, window->scr);

	window->win = XCreateWindow(
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

	XMapWindow(window->dp, window->win);

	XFlush(window->dp);

	return 0;
}

int jgrab_keyboard(window_t* window)
{
	while (XGrabKeyboard(window->dp, window->win, 1, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess);
	return 1;
}

int jnext_key_event(window_t* window, KeySym* key)
{
	XEvent ev;

	while (1) {
		XNextEvent(window->dp, &ev);

		if (ev.type != KeyPress) {
			continue;
		}

		*key = XKeycodeToKeysym(window->dp, ev.xkey.keycode, 0);
		break;
	}

	return 1;
}

void jungrab_keyboard(window_t* window)
{
	XUngrabKeyboard(window->dp, CurrentTime);
}

void jclose_window(window_t* window)
{
	XDestroyWindow(window->dp,window->win);
	XCloseDisplay(window->dp);
}
#endif
