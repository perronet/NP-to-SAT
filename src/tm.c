#ifdef DELTA
#include "delta.h"
#endif
#include "tm_lib.h"

#define INPUTFILE input_string
#define FORMULAFILE boolean_formula
#define clearConsole() printf("\e[1;1H\e[2J")
#define SLOW
#define SLEEP_TIME 50*1000

int normalizeInput(FILE * dest, FILE * src);

int countChars(FILE * f);

void getSymbols(char * dest, char * src, int len);

char_node * blankNode(char_node * prev);

bool contains(char * str, char c, int len);

// void printTransition(transition * tr);

int main(int argc, char const *argv[]){
	FILE * input = fopen("input_string", "r");
	FILE * input_clean = fopen("input_string_clean", "w+");
	FILE * state_list = fopen("state_list", "r");

	transition * tr = malloc(sizeof(transition));
	char_node * tape = malloc(sizeof(char_node));
	char_node * head;
	char curr_symbol, c;
	int i, len, curr_state, curr_steps = 0, head_offset = 0;

	tape->prev = tape;
	tape->next = NULL;
	head = tape;

	curr_state = fgetc(state_list) - 48;
	rewind(state_list);

	if(normalizeInput(input_clean, input) != -1){ //remove whitespace and newlines
		c = fgetc(input_clean);
		head->elem = c;
		curr_symbol = c;

		while((c = fgetc(input_clean)) != EOF)
			listadd(tape, c);
		rewind(input_clean);

		#ifdef FORMULA
		int len = countChars(input_clean);
		char inputstr[len+1], alphabet[len+1];
		listcpystring(tape, inputstr);
	    getSymbols(alphabet, inputstr, len); 
	    printf("Alphabet %s\n", alphabet);
	    sleep(2);
		#endif

	    while(tr->move != ACCEPT && tr->move != REJECT && tr->move != ERROR){
	    	clearConsole();
	    	printf("Computation steps: %d\n", curr_steps);
	    	printf("State: %d\n", curr_state);
		    for(i = 0; i < head_offset; ++i){
		    	printf(" ");
		    }
		    printf("V\n");
		    listprint(tape);

			tr = delta(curr_state, curr_symbol);
			curr_state = tr->state;
			head->elem = tr->symbol;

			if(tr->move == LEFT){
				if(head_offset > 0)
					head_offset--;
				head = head->prev;
			}else if(tr->move == RIGHT){
				if(head->next == NULL)
					head->next = blankNode(head);
				head_offset++;
				head = head->next;
			}

			if(tr->move != ERROR)
				curr_symbol = head->elem;

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
	    	printf("Input rejected, symbol '%c' is not recognized.\n", curr_symbol);
	    	break;

	    	default:
	    	printf("Fatal error.\n"); //should never get here
	    	break;
	    }
	    
	}else{
		printf("Error: '_' and '#' characters are not allowed in input string!\n");
	}


	#ifdef FORMULA
	//calculate boolean formula
	#endif

	free(tr);
	listdeallocate(tape);
	fclose(input);
	fclose(input_clean);
	fclose(state_list);


	return 0;
}

int normalizeInput(FILE * dest, FILE * src){ //TODO what if input empty
	char c;
	while((c = fgetc(src)) != EOF){
		if(c == '#' || c == '_'){
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

char_node * blankNode(char_node * prev){
	char_node * r = malloc(sizeof(char_node));
	r->elem = '_';
	r->prev = prev;
	r->next = NULL;

	return r;
}

bool contains(char * str, char c, int len){
	for(int i = 0; i < len; ++i){
		if(str[i] == c)
			return true;
	}

	return false;
}