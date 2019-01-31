#ifndef TM_LIB_H
#define TM_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

//Macros

#define true 1
#define false 0
#define forever for(;;)

//Structures

typedef char bool;

typedef struct char_node{ //Used mostly for the machine's tape
	char elem;
	struct char_node * prev;
	struct char_node * next;
} char_node;

typedef struct int_node{
	int elem;
	struct int_node * next;
} int_node;

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

typedef struct tm_properties{ //Resources used to reduce the problem to a boolean formula
	char_node * alphabet;
	int_node * states;
	char * input_string;
	int tot_steps; //n^k
} tm_properties;

//Functions

char_node * listadd(char_node * l, char e);

void listprint(char_node * l);

char * listcpystring(char_node * l);

void listdeallocate(char_node * l);

int listlength(char_node * l);

// char_node * listnext(char_node * n);
// char_node * listprev(char_node * n);

// int * intListToArray(int_node * l);

// bool list_contains();

// bool array_contains();

#endif
