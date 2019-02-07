#ifdef DELTA
#include "delta.h"
#endif
#include "tm_lib.h"

#define FORMULAFILE "formula"
#define clearConsole() printf("\e[1;1H\e[2J")
// #define SLOW
// #define SLEEP_TIME 5*1000
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))

#define writeClause(I, J, X) sym=X;fprintf(formula, "%d 0\n", literalId(I, J, &sym, false, prop));clauses++
#define writeClauseState(I, J, X) sym_state=X;fprintf(formula, "%d 0\n", literalId(I, J, &sym_state, true, prop));clauses++

#define writeLiteral(I, J, X) sym=X;fprintf(formula, "%d ", literalId(I, J, &sym, false, prop))
#define writeLiteralState(I, J, X) sym_state=X;fprintf(formula, "%d ", literalId(I, J, &sym_state, true, prop))
#define endClause() fprintf(formula, "0\n");clauses++

int normalizeInput(FILE * dest, FILE * src);

int countChars(FILE * f);

void getSymbols(char * dest, char * src, int len);

char_node * blankNode(char_node * prev);

bool contains(char * str, char c, int len);

void printTransition(int state, char symbol, transition * tr);

//Given cell coordinates and cell symbol returns unique id for that boolean varaible
//Cell symbols = {alphabet}U{states}
int literalId(int i, int j, void * s, bool isState, tm_properties * prop); //s can be a symbol or state
int stateId(int s, tm_properties * prop);
int symbolId(char c, tm_properties * prop);
int maxLiteralId(tm_properties * prop);

int main(int argc, char const *argv[]){
	FILE * input = fopen("input_string", "r");
	FILE * input_clean = fopen("input_string_clean", "w+");
	FILE * state_list = fopen("state_list", "r");

	transition * tr = malloc(sizeof(transition));
	char_node * tape = malloc(sizeof(char_node));
	char_node * head;
	tm_properties * prop; //data needed to generate formula
	char curr_symbol, c;
	int i, j, len, curr_state, curr_steps = 0, head_offset = 0;

	tape->prev = tape;
	tape->next = NULL;
	head = tape;

	curr_state = fgetc(state_list) - 48; //TODO it could be more than one digit
	rewind(state_list);

	if(normalizeInput(input_clean, input) != -1){ //remove whitespace and newlines
		c = fgetc(input_clean);
		head->elem = c;
		curr_symbol = c;

		while((c = fgetc(input_clean)) != EOF)
			listadd(tape, c);
		rewind(input_clean);

		#ifdef FORMULA
			//Calculate input alphabet
			prop = malloc(sizeof(tm_properties));
			len = countChars(input_clean);
			prop->alphabet = malloc(sizeof(char)*(len+3)); //# and _ are included
			prop->input_string = malloc(sizeof(char)*(len+1));
			listcpystring(tape, prop->input_string);
		    getSymbols(prop->alphabet, prop->input_string, len); 
		    prop->alphabet_length = strlen(prop->alphabet);

		    //Get state list
			// len = countLines(state_list)-1; //TODO it could be more than one digit
			len = countChars(state_list);
			prop->states_length = len+2; //-1 and -2 (reject and accept) are included
			prop->states = malloc((len+2)*sizeof(int));
			for(i = 0; i < len; ++i){
				prop->states[i] = fgetc(state_list) - 48;
			}
			prop->states[i] = -1;
			prop->states[i+1] = -2;
			sleep(1);
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
		    printf("\n");

			tr = delta(curr_state, curr_symbol);
			printTransition(curr_state, curr_symbol, tr);
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

	free(tr);
	listdeallocate(tape);
	fclose(input);
	fclose(input_clean);
	fclose(state_list);

	#ifdef FORMULA
	FILE * formula = fopen(FORMULAFILE, "w");
	prop->tot_steps = curr_steps;
	printProperties(prop);

	//Calculate the 4 parts of the formula
	int clauses = 0, input_length = strlen(prop->input_string); 
	int sym_state, accept_state = prop->states[prop->states_length-1];
	char sym;

	//Phi-start
	fprintf(formula, "p cnf %d %d\n", maxLiteralId(prop), prop->tot_steps+4); //syntax: p cnf n_variables n_clauses
	fprintf(formula, "c Phi-start\n");										  //TODO write here clause number
	writeClause(1, 1, '#');
	writeClauseState(1, 2, prop->states[0]);
	fflush(formula);
	for(j = 0; j < input_length; ++j){
		writeClause(1, j+3, prop->input_string[j]); //+3 needed because there are always 2 delimiters '#' and 1 state
	}
	for(j; j < prop->tot_steps; ++j){
		writeClause(1, j+3, '_');
	}
	writeClause(1, j+3, '#');
	fflush(formula);

	//Phi-accept
	fprintf(formula, "c Phi-accept\n");
	for(i = 1; i <= prop->tot_steps; ++i){
		for(j = 1; j <= prop->tot_steps+3; ++j){
			writeLiteralState(i, j, accept_state);
		}
	}
	endClause();

	//Phi-cell

	//Phi-move

	// char test = 'b';
	// int test2 = 0;
	// for(int j=1; j <= prop->tot_steps+3; ++j){
	// 	printf("j:%d %d\n", j, literalId(1, j, &test, false, prop));
	// }
	#endif

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

void getSymbols(char * dest, char * src, int len){
	int i = 0, k = 2;
	dest[0] = '#';
	dest[1] = '_';
	for(i; i < len && src[i] != '\0'; ++i){
		if(!contains(dest, src[i], i+3)){
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

void printTransition(int state, char symbol, transition * tr){
	char strmove[7];
	enumToString(strmove, tr->move);
	printf("Transition: %d, %c -> %d, %c, %s\n\n", state, symbol, tr->state, tr->symbol, strmove);
}

bool contains(char * str, char c, int len){
	for(int i = 0; i < len; ++i){
		if(str[i] == c)
			return true;
	}

	return false;
}

//i,j are 1-based, cell_sym is 0-based
//the set of possible cell symbols will be formed like this:
//"#,_, ...alphabet..., ...states..., -1, -2" where -1 and -2 are reject and accept
int literalId(int i, int j, void * s, bool isState, tm_properties * prop){
	int cell_sym_len = prop->alphabet_length + prop->states_length;
	int s_id, offset;

	if(isState)
		s_id = prop->alphabet_length + stateId(*(int *)s, prop);
	else
		s_id = symbolId(*(char *)s, prop);
	
	offset = -cell_sym_len+s_id+1;
	return (i-1)*(prop->tot_steps+3)+(j*cell_sym_len)+offset;
}

int stateId(int s, tm_properties * prop){
	for(int i = 0; i < prop->states_length; ++i){
		if (prop->states[i] == s)
			return i;
	}

	return -1;
}

int symbolId(char c, tm_properties * prop){
	for(int i = 0; i < prop->alphabet_length; ++i){
		if (prop->alphabet[i] == c)
			return i;
	}

	return -1;
}

int maxLiteralId(tm_properties * prop){ //the highest literal id in the bottom right corner, it will always be the accept state
	int sym_state = prop->states[prop->states_length-1];
	return literalId(prop->tot_steps, prop->tot_steps+3, &sym_state, true, prop);
}