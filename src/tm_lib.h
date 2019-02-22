#ifndef TM_LIB_H
#define TM_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

//Macros

#define MAX_INT_DIGITS countDigitsInt(INT_MAX)
#define MAX_LONG_DIGITS countDigitsLong(LONG_MAX)
#define true 1
#define false 0
#define decodeStateId(X) (abs(X)-1)
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

//Structures

typedef char bool;

typedef struct char_node{ //Used for the machine's tape
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
} window_node;

enum action{
    LEFT,
    RIGHT,
    ACCEPT, //represented as -2 in the states array
    REJECT, //represented as -1 in the states array
    ERROR 
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
    int table_height;
    int table_width;
} tm_properties;

//Functions

//List
char_node * listadd(char_node * l, char e);
window_node * addWindow(window_node * last);
permutation_node * addPermutation(permutation_node * last);
int listlength(char_node * l);
int listlengthWindows(window_node * l);
void listcpystring(char_node * l, char * str);
char * listcpystring_new(char_node * l);

//Prints
void listprint(char_node * l);
void printWindows(window_node * l);
void printPermutations(permutation_node * l);
void printProperties(tm_properties * prop);
void printarray(int * a, int len);
void printSingleWindow(int * w);
void printSinglePermutation(int * p);
void printTransition(int state, char symbol, transition * tr);

//File manipulation
int countChars(FILE * f);
int countLines(FILE * f);
int readInt(FILE * f, char * dest);
void writeInt(FILE * f, int n);
void writeLong(FILE * f, long n);

//Deallocators
void listdeallocatechar(char_node * l);
void listdeallocateperm(permutation_node * l);
void listdeallocatewin(window_node * l);

//Utils
char_node * blankNode(char_node * prev);
bool contains(char * str, char c, int len);
char * enumToString(char * str, enum action a);
int countDigitsInt(int n);
int countDigitsLong(long n);

#endif
