#include "line_buf.h"
#include <string.h>

static int
utf8_tail(char byte)
{
	return (byte & 0xc0) == 0x80;
}

void
append_text(struct line_buf *line, struct text *text)
{
	if (line->pos + text->size > sizeof(line->buf))
		return;

	memcpy(line->buf + line->pos, text->buf, text->size);
	line->pos += text->size;
}

void
backspace(struct line_buf *line)
{
	char *byte = line->buf + line->pos - 1;

	if (line->pos == 0) return;

	while (utf8_tail(*byte)) {
		byte--;
	}

	line->pos = byte - line->buf;
}
