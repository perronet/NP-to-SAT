#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define true 1
#define false 0
#define forever for(;;)

typedef char bool;

enum action{
	LEFT,
	RIGHT,
	ACCEPT,
	REJECT,
	ERROR
};

typedef struct curr_status{
	int state;
	char symbol;
} curr_status;

typedef struct transition{
	int state;
	char symbol;
	enum action move;
} transition;
