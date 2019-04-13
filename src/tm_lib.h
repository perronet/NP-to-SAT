#ifndef TM_LIB_H
#define TM_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

/* Macros */

#define MAX_INT_DIGITS count_digits_int(INT_MAX)
#define MAX_LONG_DIGITS count_digits_long(LONG_MAX)
#define MAX_SLEEP_TIME_DIGITS 5
#define DEFAULT_SLEEP_TIME "500"
#define true 1
#define false 0
#define decodeStateId(X) (abs(X)-1)
#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))

/* Structures */

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

/* Functions */

//List
char_node * add_char(char_node * l, char e);
window_node * add_window(window_node * last);
permutation_node * add_permutation(permutation_node * last);
int list_len_char(char_node * l);
int list_len_window(window_node * l);
void tape_to_str(char_node * l, char * str);
char * tape_to_str_new(char_node * l);

//Prints
void print_chars(char_node * l);
void print_windows(window_node * l);
void print_single_window(int * w);
void print_permutations(permutation_node * l);
void print_single_permutation(int * p);
void print_properties(tm_properties * prop);
void print_array(int * a, int len);
void print_tape(char_node * tape, int head_offset);
void print_transition(int state, char symbol, transition * tr);

//File manipulation
int count_chars(FILE * f);
int count_lines(FILE * f);
int read_int(FILE * f, char * dest);
void write_int(FILE * f, int n);
void write_long(FILE * f, long n);

//Deallocators
void list_dealloc_char(char_node * l);
void list_dealloc_permutations(permutation_node * l);
void list_dealloc_windows(window_node * l);

//Utils
char_node * blank_node(char_node * prev);
bool contains(char * str, char c, int len);
char * enum_to_string(char * str, enum action a); //replace this with macro
int count_digits_int(int n);
int count_digits_long(long n);

#endif
