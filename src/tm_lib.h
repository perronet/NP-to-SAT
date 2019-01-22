#ifndef TM_LIB_H//include guard
#define TM_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
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
	char state;
	char symbol;
} curr_status;

typedef struct transition{
	char state;
	char symbol;
	enum action move;
} transition;

typedef struct tm_properties{
	//alphabet
	//states
	//input_string
	int tot_steps;
} tm_properties;

//listcpy

//deleteDuplicates

#endif
