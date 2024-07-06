#ifndef LINE_BUF_H
#define LINE_BUF_H

#define LINE_BUF_MAX 200

struct text {
	char buf[64];
	int  size;
};

struct stack {
	int top;
	char buf[LINE_BUF_MAX];
};

struct line_buf {
	struct stack sizes;
	int  pos;
	char buf[LINE_BUF_MAX];
};

extern void append_text(struct line_buf*, struct text*);
extern void backspace(struct line_buf*);

#endif
