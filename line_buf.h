#ifndef LINE_BUF_H
#define LINE_BUF_H

#include <stddef.h>

struct text {
	size_t size;
	char   buf[64];
};

struct line_buf {
	size_t pos;
	char   buf[200];
};

extern void append_text(struct line_buf*, struct text*);
extern void backspace(struct line_buf*);

#endif
