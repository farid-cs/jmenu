#ifndef XSERVER_H
#define XSERVER_H


#include <X11/Xlib.h>
#include <X11/Xutil.h>
typedef struct {
	Display* dp;
	Window   win;
	Window   root;
	int      scr;
	int      err;
} window_t;

extern int  jopen_window(window_t*, const int, const int, const int, const int, const int);
extern int  jgrab_keyboard(window_t*);
extern int  jnext_key_event(window_t*, KeySym*);
extern void jungrab_keyboard(window_t*);
extern void jclose_window(window_t*);
#endif
