#define _GNU_SOURCE
#include <stdio.h>
#include "liballocs.h"
#include "relf.h"

static int cb(void *ip, void *sp, void *bp, void *arg)
{
	const char *sname = ip ? "(unknown)" : "(no active function)";
	int ret = ip ? fake_dladdr(ip, NULL, NULL, &sname, NULL) : 0;
	printf("%s\n", sname);
	return 0; // keep going
}

int (__attribute__((optimize("O0"))) h)(void)
{
	__liballocs_walk_stack(cb, NULL);
	return 0;
}

int (__attribute__((optimize("O0"))) g)(void)
{
	return h();
}


int (__attribute__((optimize("O0"))) f)(void)
{
	return g();
}


int main(void)
{
	return f();
}
