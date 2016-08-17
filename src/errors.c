#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

void die(const char* message)
{
	fprintf(stderr, message);
	exit(1);
}

