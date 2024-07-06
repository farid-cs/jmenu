#ifndef X11_H
#define X11_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include "line_buf.h"

extern int    x11_connect();
extern void   x11_disconnect();
extern int    open_window(int, int, int, int, int);
extern void   close_window();
extern int    grab_keyboard();
extern void   ungrab_keyboard();
extern int    set_font(const char*);
extern int    font_height();
extern int    set_fg_color(const char*);
extern int    set_bg_color(const char*);
extern void   free_font();
extern void   free_colors();
extern int    create_draw();
extern int    draw_string(const char*, const int);
extern void   free_draw();
extern int    next_event();
extern XEvent event();
extern KeySym which_key();
extern struct text*   key_to_text();
extern int    clear_window();

#endif
