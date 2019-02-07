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
	ACCEPT, //encoded as -2 in the formula
	REJECT, //encoded as -1 in the formula
	ERROR   //encoded as -1 in the formula
};

typedef struct transition{
	int state;
	char symbol;
	enum action move;
} transition;

//Resources used to reduce the problem to a boolean formula
typedef struct tm_properties{ 
	char * alphabet;
	int * states;
	char * input_string;
	int alphabet_length;
	int states_length;
	int tot_steps; //used to put an upper bound to the computation steps
} tm_properties;

//Functions

char_node * listadd(char_node * l, char e);

void listprint(char_node * l);

void listcpystring(char_node * l, char * str);

char * listcpystring_new(char_node * l);

void listdeallocate(char_node * l);

int listlength(char_node * l);

char * enumToString(char * str, enum action a);

void printProperties(tm_properties * prop);

void printarray(int * a, int len);

int countChars(FILE * f);
int countLines(FILE * f);

// int * intListToArray(int_node * l);

// bool list_contains();

// bool array_contains();

#endif
