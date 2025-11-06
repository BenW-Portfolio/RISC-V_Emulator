/*
 * Copyright (c) Gabriel Parmer, 2024.
 * Do not copy nor distribute without permission.
 */

/* One cache-line here, second in the stack. */
long global;

int
main(void)
{
	global = 42;

	return 0;
}
