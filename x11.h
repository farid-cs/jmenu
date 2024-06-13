#ifndef X11_H
#define X11_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
typedef struct {
	Display* dp;
	Window   id;
	GC       gc;
	Window   root;
	XftFont* font;
	int      scr;
} window_t;

typedef struct {
	unsigned int  key;
	unsigned long state;
} key_event_t;

extern int  open_window(window_t*, const int, const int, const int, const int, const int);
extern int  grab_keyboard(window_t*);
extern int  next_key_event(window_t*, key_event_t*);
extern void ungrab_keyboard(window_t*);
extern int  set_font(window_t*, const char*);
extern void free_font(window_t*);
extern int  draw_string(window_t* window, const char* string);
extern void close_window(window_t*);
#endif
