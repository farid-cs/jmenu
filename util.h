#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
static void eputs(const char* err_msg)
{
	fputs(err_msg,stderr);
	putc('\n', stderr);
}

#endif
