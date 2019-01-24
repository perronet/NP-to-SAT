#ifdef DELTA
#include "delta.h"
#endif
#include "tm_lib.h"

#define INPUTFILE input_string
#define FORMULAFILE boolean_formula

int main(int argc, char const *argv[]){
	transition * r = malloc(sizeof(transition));
	char_node * tape = malloc(sizeof(char_node));
	char_node * head;

	#ifdef FORMULA
	//calculate boolean formula
	#endif

	#ifdef SLOW
	//use slow mode
	#endif

	tape->elem = 'a';
	tape->prev = tape;
	tape->next = NULL;
	head = tape;

	//Read input string (first remove whitespace)

	return 0;
}

	// listadd(tape, 'b');
	// listadd(tape, 'c');
	// listadd(tape, 'd');
	// listadd(tape, 'e');
	// listprint(tape);
	// char * copy = listcpystring(tape);
	// printf("%s\n", copy);
	// listdeallocate(tape);