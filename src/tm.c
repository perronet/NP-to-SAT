#ifdef DELTA
#include "delta.h"
#endif
#include "tm_lib.h"

#define INPUTFILE input_string
#define FORMULAFILE boolean_formula
#define clearConsole() printf("\e[1;1H\e[2J")
#define SLOW
#define SLEEP_TIME 300*1000

int normalizeInput(FILE * dest, FILE * src);

int countChars(FILE * f);

void getSymbols(char * dest, char * src, int len);

bool contains(char * str, char c, int len);

// void printTransition(transition * tr);

int main(int argc, char const *argv[]){
	FILE * input = fopen("input_string", "r");
	FILE * input_clean = fopen("input_string_clean", "w+");
	FILE * state_list = fopen("state_list", "r");

	transition * tr = malloc(sizeof(transition));
	char_node * tape = malloc(sizeof(char_node));
	char_node * head;
	char curr_symbol;
	int i, len, curr_state, curr_steps = 0, head_offset = 0;

	tape->elem = 'a';
	tape->prev = tape;
	tape->next = NULL;
	head = tape;

	curr_state = fgetc(state_list) - 48;
	rewind(state_list);

	if(normalizeInput(input_clean, input) != -1){ //remove whitespace and newlines
		len = countChars(input_clean);
		char inputstr[len]; 
		for (i = 0; i < len; ++i){
			inputstr[i] = fgetc(input_clean); 
		}
		inputstr[i] = '\0'; 
		curr_symbol = inputstr[0];

		#ifdef FORMULA
	    char alphabet[len];
	    getSymbols(alphabet, inputstr, len); 
	    printf("Alphabet %s\n", alphabet);
		#endif

	    while(tr->move != ACCEPT && tr->move != REJECT && tr->move != ERROR){
	    	clearConsole();
	    	printf("Computation steps: %d\n", curr_steps);
	    	printf("State: %d\n", curr_state);
		    for(i = 0; i < head_offset; ++i){
		    	printf(" ");
		    }
		    printf("V\n");
			printf("%s\n\n", inputstr);

			tr = delta(curr_state, curr_symbol);
			curr_state = tr->state;
			inputstr[head_offset] = tr->symbol;

			if(tr->move == LEFT){
				if(head_offset > 0)
					head_offset--;
			}else if(tr->move == RIGHT){ //could get off the array
				head_offset++;
			}

			if(tr->move != ERROR)
				curr_symbol = inputstr[head_offset];

			curr_steps++;
			#ifdef SLOW
			usleep(SLEEP_TIME);
			#endif
	    }

	    switch(tr->move){
	    	case ACCEPT:
	    	printf("Input accepted.\n");
	    	break;

	    	case REJECT:
	    	printf("Input rejected.\n");
	    	break;

	    	case ERROR:
	    	printf("Input rejected, symbol '%c' is not recognized\n", curr_symbol);
	    	break;

	    	default:
	    	printf("Fatal error\n"); //should never get here
	    	break;
	    }

	}else{
		printf("Error: '_' and '#' characters are not allowed!\n");
	}


	#ifdef FORMULA
	//calculate boolean formula
	#endif

	return 0;
}

int normalizeInput(FILE * dest, FILE * src){
	char c;
	while((c = fgetc(src)) != EOF){

		if(c == '#'){ //REMINDER: re add _ after testing
			return -1;
		}else if(c != ' ' && c != '\n'){
			fprintf(dest, "%c", c);
		}
    }

    rewind(dest);
    return 0;
}

int countChars(FILE * f){
	char c;
	int r = 0;
	c = fgetc(f);
	while(c != EOF){
		c = fgetc(f);
		r++;
	}
	rewind(f);

	return r;
}

void getSymbols(char * dest, char * src, int len){
	int i = 0, k = 0;
	for(i; i < len && src[i] != '\0'; ++i){
		if(!contains(dest, src[i], i+1)){
			dest[k] = src[i];
			k++;
		}
	}
	dest[k] = '\0';
}

bool contains(char * str, char c, int len){
	for(int i = 0; i < len; ++i){
		if(str[i] == c)
			return true;
	}

	return false;
}

	// listadd(tape, 'b');
	// listadd(tape, 'c');
	// listadd(tape, 'd');
	// listadd(tape, 'e');
	// listprint(tape);
	// char * copy = listcpystring(tape);
	// printf("%s\n", copy);
	// listdeallocate(tape);