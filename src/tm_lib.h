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
#define decodeStateId(X) (abs(X))

//Structures

typedef char bool;

typedef struct char_node{ //Used mostly for the machine's tape
	char elem;
	struct char_node * prev;
	struct char_node * next;
} char_node;

typedef struct permutation_node{
	int permutation[3];
	struct permutation_node * next;
} permutation_node;

typedef struct window_node{
	int window[6];
	struct window_node * next;
}window_node;

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

int listlength(char_node * l);

char * enumToString(char * str, enum action a);

void printProperties(tm_properties * prop);

void printarray(int * a, int len);

void printSingleWindow(int * w);

void printSinglePermutation(int * p);

int countChars(FILE * f);
int countLines(FILE * f);

void listdeallocatechar(char_node * l);
void listdeallocateperm(permutation_node * l);
void listdeallocatewin(window_node * l);

window_node * addWindow(window_node * last);
void printWindows(window_node * l);
permutation_node * addPermutation(permutation_node * last);
void printPermutations(permutation_node * l);

// int * intListToArray(int_node * l);

// bool list_contains();

// bool array_contains();

#endif
