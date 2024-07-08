#include <stdio.h>
#include "util.h"

void eputs(const char* err_msg)
{
	fputs(err_msg, stderr);
	putc('\n', stderr);
}
