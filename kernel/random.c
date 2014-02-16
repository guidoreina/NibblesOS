#include "kernel/random.h"

unsigned random(void)
{
	static unsigned x = 123456789;
	static unsigned y = 362436069;
	static unsigned z = 521288629;
	static unsigned w = 88675123;
	static unsigned t;

	t = x ^ (x << 11);
	x = y;
	y = z;
	z = w;
	w = w ^ (w >> 19) ^ (t ^ (t >> 8));

	return w;
}
