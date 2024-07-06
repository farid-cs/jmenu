#include "line_buf.h"
#include <string.h>

static void push(struct stack*, int val);
static int  pop(struct stack*);

void append_text(struct line_buf* line, struct text* text)
{
	if (line->pos + text->size > sizeof(line->buf))
		return;

	memcpy(line->buf + line->pos, text->buf, text->size);

	line->pos += text->size;
	push(&line->sizes, text->size);
}

void backspace(struct line_buf* line)
{
	if (line->pos == 0) return;

	line->pos -= pop(&line->sizes);
}

void push(struct stack* stack, int val)
{
	if (stack->top < sizeof(stack->buf))
		stack->buf[stack->top++] = val;
}

int pop(struct stack* stack)
{
	if (stack->top > 0)
		return stack->buf[--stack->top];
	return 0;
}
