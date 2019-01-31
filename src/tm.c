#ifdef DELTA
#include "delta.h"
#endif
#include "tm_lib.h"

#define INPUTFILE input_string
#define FORMULAFILE boolean_formula

int normalizeInput(FILE * src, FILE * dest);

int main(int argc, char const *argv[]){
	FILE * input = fopen("input_string", "r");
	FILE * input_clean = fopen("input_clean", "w+");

	transition * r = malloc(sizeof(transition));
	char_node * tape = malloc(sizeof(char_node));
	char_node * head;

	//remove whitespace
	if(normalizeInput(input, input_clean) != -1){
		
	}else{
		printf("Error: '_' and '#' characters are not allowed!\n");
	}


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

int normalizeInput(FILE * src, FILE * dest){
	char c;
	while((c = fgetc(src)) != EOF){
		if(c == ' '){
            while(c == ' '){
                c = fgetc(src);
            }
            if(c != EOF)
                fseek(src, -1, SEEK_CUR);
		}else if(c == '#' || c == '_'){
			return -1;
		}else{
			fprintf(dest, "%c", c);
		}
    }

    rewind(dest);
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