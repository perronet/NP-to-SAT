#ifdef DELTA
#include "delta.h"
#endif
#include "tm_lib.h"

#define FORMULAFILE "formula"
// #define SLOW
// #define SLEEP_TIME 50*1000

#define clearConsole() printf("\e[1;1H\e[2J")
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))
#define isState(X) (X >= prop->alphabet_length)
#define isStateWindow(X) (X < 0)
#define encodeStateId(X) (-(X+1))
#define decodeStateId(X) (abs(X)-1)
#define isAccOrRejState(X) (isStateWindow(X) && decodeStateId(X) >= cell_sym_len-2)
#define evaluateWindowDnfClause() (clause[0] && clause[1] && clause[2] && clause[3] && clause[4] && clause[5])

#define writeTopRow() win_ptr->window[0]=perm_ptr->permutation[0];win_ptr->window[1]=perm_ptr->permutation[1];win_ptr->window[2]=perm_ptr->permutation[2]
#define writeBottomRow(A, B, C) win_ptr->window[3]=A;win_ptr->window[4]=B;win_ptr->window[5]=C
#define writeWindow(A, B, C) writeTopRow();writeBottomRow(A, B, C);win_ptr=addWindow(win_ptr)

#define writeLiteralId(I, J, X) fprintf(formula, "%d ", literalIdGiven(I, J, X, prop));literals++
#define writeLiteralIdNegated(I, J, X) fprintf(formula, "-%d ", literalIdGiven(I, J, X, prop));literals++
#define writeLiteral(I, J, X) sym=X;fprintf(formula, "%d ", literalId(I, J, &sym, false, prop));literals++
#define writeLiteralState(I, J, X) sym_state=X;fprintf(formula, "%d ", literalId(I, J, &sym_state, true, prop));literals++
#define endClause() fprintf(formula, "0\n");clauses++

int normalizeInput(FILE * dest, FILE * src);

int countChars(FILE * f);

void getSymbols(char * dest, char * src, int len);

char_node * blankNode(char_node * prev);

bool contains(char * str, char c, int len);

void printTransition(int state, char symbol, transition * tr);

//Given cell coordinates and cell symbol returns unique id for that boolean varaible
//Cell symbols = {alphabet}U{states}
int literalId(int i, int j, void * s, bool state, tm_properties * prop); //s can be a symbol or state
int literalIdGiven(int i, int j, int s_id, tm_properties * prop);          //symbol id is given
int stateId(int s, tm_properties * prop);
int symbolId(char c, tm_properties * prop);
int maxLiteralId(tm_properties * prop);

void calculatePermutations(permutation_node * l, tm_properties * prop);
void calculateLegalWindows(window_node * legal_windows, tm_properties * prop);
void writeWindowsCnf(FILE * f, window_node * w, tm_properties * prop);
void toBinaryString(char * dest, int n, int len);
bool evaluateWindowDnfExpression(char * str, window_node * w, tm_properties * prop);

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
	    	printf("Input accepted.\n\n");
	    	break;

	    	case REJECT:
	    	printf("Input rejected.\n\n");
	    	break;

	    	case ERROR:
	    	printf("Input rejected, symbol '%c' is not recognized.\n\n", curr_symbol);
	    	break;

	    	default:
	    	printf("Fatal error.\n\n"); //should never get here
	    	break;
	    }
	    
	}else{
		printf("Error: '_' and '#' characters are not allowed in input string!\n");
	}

	free(tr);
	listdeallocatechar(tape);
	fclose(input);
	fclose(input_clean);
	fclose(state_list);

#ifdef FORMULA

	FILE * formula = fopen(FORMULAFILE, "w");
	FILE * windows_cnf = fopen("windows_cnf", "w+");
	prop->tot_steps = curr_steps;
	printProperties(prop);
	printf("Calculating formula ...\n");

	//Calculate the 4 parts of the formula
	int k, h, lines, clauses = 0, literals = 0, input_length = strlen(prop->input_string); 
	int accept_state = prop->states[prop->states_length-1];
	int cell_sym_len = prop->alphabet_length + prop->states_length; //total number of possible symbols in a cell
	int sym_state;  //used to write a cell symbol correspoding to a state
	char sym;		//used to write a cell symbol correspoding to an alphabet symbol

	//Phi-start
	fprintf(formula, "p cnf %d %d\n", maxLiteralId(prop), prop->tot_steps+4); //syntax: p cnf n_variables n_clauses
	fprintf(formula, "c ##### Phi-start #####\n");				              //TODO write here clause number
	writeLiteral(1, 1, '#');
	endClause();
	writeLiteralState(1, 2, prop->states[0]);
	endClause();
	fflush(formula);
	for(j = 0; j < input_length; ++j){
		writeLiteral(1, j+3, prop->input_string[j]); //+3 needed because there are always 2 delimiters '#' and 1 state
		endClause();
	}
	for(j; j < prop->tot_steps; ++j){
		writeLiteral(1, j+3, '_');
		endClause();
	}
	writeLiteral(1, j+3, '#');
	endClause();
	fflush(formula);

	//Phi-accept
	fprintf(formula, "c ##### Phi-accept #####\n");
	for(i = 1; i <= prop->tot_steps; ++i){
		for(j = 1; j <= prop->tot_steps+3; ++j){
			writeLiteralState(i, j, accept_state);
			// writeLiteralId(i, j, cell_sym_len-1);
		}
	}
	endClause();
	fflush(formula);

	//Phi-cell 
	fprintf(formula, "c ##### Phi-cell #####\n");
	for(i = 1; i <= prop->tot_steps; ++i){
		for(j = 1; j <= prop->tot_steps+3; ++j){
			fprintf(formula, "c ## Cell [%d,%d] ##\n", i, j);

			for(k = 0; k < cell_sym_len; ++k){
				writeLiteralId(i, j, k);
			}
			endClause();


			for(k = 0; k < cell_sym_len; ++k){
				for(h = 0; h < cell_sym_len; ++h){
					if(h != k){
						writeLiteralIdNegated(i, j, k);
						writeLiteralIdNegated(i, j, h);
						endClause();
					}
				}
			}

		}
	}	
	fflush(formula);

	//Phi-move
	fprintf(formula, "c ##### Phi-move #####\n");
	window_node * legal_windows = malloc(sizeof(window_node));
	calculateLegalWindows(legal_windows, prop);

	//Write and convert legal windows in conjunctive normal form
	writeWindowsCnf(windows_cnf, legal_windows, prop);
	lines = countLines(windows_cnf);

	for(i = 1; i < prop->tot_steps; ++i){
		for(j = 2; j < prop->tot_steps+3; ++j){
			fprintf(formula, "c ## Window [%d,%d] ##\n", i, j);
			for(k = 0; k < lines-1; ++k){ //parse ids from file
				for(h = 0; h < cell_sym_len; ++h){
					c = fgetc(windows_cnf);
					if(c != '!'){
						writeLiteralId(i, j, h);
					}else{
						writeLiteralIdNegated(i, j, h);
						c = fgetc(windows_cnf); //skip this literal and go to the next
					}
				}
				endClause();
				c = fgetc(windows_cnf); //go to next line
			}
			rewind(windows_cnf);
		}
	}
	fflush(formula);
	printf("Done.\n");
	printf("Literals:%d Clauses:%d\n", literals, clauses);

	fclose(windows_cnf);
	fclose(formula);

#endif//FORMULA

	return 0;
}

void writeWindowsCnf(FILE * f, window_node * w, tm_properties * prop){
    int i, j, cell_sym_len = prop->alphabet_length + prop->states_length;
	bool table_row[cell_sym_len+1];

	//Evaluate each row of truth table
	for(i = 0; i < powint(2, cell_sym_len); ++i){
		toBinaryString(table_row, i, cell_sym_len);	
		if(!evaluateWindowDnfExpression(table_row, w, prop)){ //let's write x if the literal is false and !x if it's true (we write ids)
			for(j = 0; j < cell_sym_len; ++j){
				if(table_row[j])
					fprintf(f, "!%d", j);
				else
					fprintf(f, "%d", j);
				
			}
			fprintf(f, "\n");
		}
	}
	fflush(f);
	rewind(f);
}

bool evaluateWindowDnfExpression(bool * truth_values, window_node * w, tm_properties * prop){
	bool clause[7]; //for each window assign truth values then evaluate
	while(w->next != NULL){
		// printSingleWindow(w->window);
		for(int i = 0; i < 6; ++i){
			if(isStateWindow(w->window[i]))
				clause[i] = truth_values[decodeStateId(w->window[i]) + prop->alphabet_length];
			else
				clause[i] = truth_values[symbolId(w->window[i], prop)];
		}
		clause[6] = '\0';
		w = w->next;

		if(evaluateWindowDnfClause())
			return true;
	}

	return false;
}

void toBinaryString(bool * dest, int n, int len){
    for(int b = powint(2, len-1); b > 0; b >>= 1){ //powint(2, len-1) is the msb
        *dest = (((n & b) == b) ? true : false);
        dest++;
    }
    *dest = '\0';
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
			// if(src[i] != 'x'){//TODO test
			dest[k] = src[i];
			k++;
			// }
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
//TODO refactor this to use isState macro and take as input s an int and treat it as a char
int literalId(int i, int j, void * s, bool state, tm_properties * prop){
	int cell_sym_len = prop->alphabet_length + prop->states_length;
	int s_id, offset;

	if(state)
		s_id = prop->alphabet_length + stateId(*(int *)s, prop);
	else
		s_id = symbolId(*(char *)s, prop);
	
	offset = -cell_sym_len+s_id+1;
	return (i-1)*(prop->tot_steps+3)+(j*cell_sym_len)+offset;
}

//Symbol/state id is given
int literalIdGiven(int i, int j, int s_id, tm_properties * prop){
	int cell_sym_len = prop->alphabet_length + prop->states_length;
	int offset = -cell_sym_len+s_id+1;

	return (i-1)*(prop->tot_steps+3)+(j*cell_sym_len)+offset;
}

//Returns state index
int stateId(int s, tm_properties * prop){
	for(int i = 0; i < prop->states_length; ++i){
		if (prop->states[i] == s)
			return i;
	}

	return -1;
}

//Returns symbol index
int symbolId(char c, tm_properties * prop){
	for(int i = 0; i < prop->alphabet_length; ++i){
		if (prop->alphabet[i] == c)
			return i;
	}

	return -1;
}

//The highest literal id is in the bottom right corner, it will always be the accept state
int maxLiteralId(tm_properties * prop){ 
	int sym_state = prop->states[prop->states_length-1];
	return literalId(prop->tot_steps, prop->tot_steps+3, &sym_state, true, prop);
}

void calculateLegalWindows(window_node * legal_windows, tm_properties * prop){
	window_node * win_ptr = legal_windows;
	int i, j, acc_state_id = prop->states_length-1, rej_state_id = prop->states_length-2; 
    int cell_sym_len = prop->alphabet_length + prop->states_length;
	transition * tr = malloc(sizeof(transition));
	permutation_node * permutations = malloc(sizeof(permutation_node)); 
	permutation_node * perm_ptr = permutations;

	//Permutations and windows are actually integers arrays treated as char arrays, where negative integers are states
	calculatePermutations(permutations, prop);

	while(perm_ptr->next != NULL){

		//CASE 1: There are no states in the permutation
		if(!isStateWindow(perm_ptr->permutation[0]) && 
		   !isStateWindow(perm_ptr->permutation[1]) && 
		   !isStateWindow(perm_ptr->permutation[2]))
		{
			//'_' in these comments means an information we don't care about
			//Symbols remain the same (trivial)
			writeWindow(perm_ptr->permutation[0], perm_ptr->permutation[1], perm_ptr->permutation[2]);

			//State appears from the left
			//See if there are transitions (_,s[0])->(qx,_,R) 
			for(i = 0; i < prop->states_length-2; ++i){ //exclude accept and reject
				tr = delta(prop->states[i], perm_ptr->permutation[0]);
				if(tr->move == RIGHT){
					writeWindow(encodeStateId(stateId(tr->state, prop)), perm_ptr->permutation[1], perm_ptr->permutation[2]);
				}
			}

			//Only left symbol is modified
			//See if there are transitions (_,s[0])->(_,s,L)
			for(i = 0; i < prop->states_length-2; ++i){ //exclude accept and reject
				tr = delta(prop->states[i], perm_ptr->permutation[0]);
				if(tr->move == LEFT){
					writeWindow(tr->symbol, perm_ptr->permutation[1], perm_ptr->permutation[2]);
				}
			}

			//State appears from the right (added exception for delimiter '#')
			//See if there are transitions (_,_)->(qx,_,L)
			if(perm_ptr->permutation[2] != '#'){ //can't appear from the right if there's a delimiter
				for(i = 0; i < prop->states_length-2; ++i){
					for(j = 0; j < prop->alphabet_length; ++j){
						tr = delta(prop->states[i], prop->alphabet[j]);
						if(tr->move == LEFT){
							writeWindow(perm_ptr->permutation[0], perm_ptr->permutation[1], encodeStateId(stateId(tr->state, prop)));
						}
					}
				}
			}

		//CASE 2: States are present in the permutation
		}else{
			if(isAccOrRejState(perm_ptr->permutation[0]) ||
			   isAccOrRejState(perm_ptr->permutation[1]) ||
			   isAccOrRejState(perm_ptr->permutation[2]))
			{
				//Symbols remain the same, this is a special "static" transition for acc and rej
				writeWindow(perm_ptr->permutation[0], perm_ptr->permutation[1], perm_ptr->permutation[2]);
			}else{

				//State is in the left side
				if(isStateWindow(perm_ptr->permutation[0])){
					tr = delta(prop->states[decodeStateId(perm_ptr->permutation[0])], perm_ptr->permutation[1]);
					switch(tr->move){
						//State disappears to the left, unknown symbol appears from the left (but it can't be the delimiter '#')
						//See if there are transitions (q[0],s[1])->(_,s,L)
						case LEFT:
							for(j = 1; j < prop->alphabet_length; ++j){ //start from 1 to exclude delimiter
								writeWindow(prop->alphabet[j], tr->symbol, perm_ptr->permutation[2]);	
							}
						break;

						//State moves from left to center
						//See if there are transitions (q[0],s[1])->(q,s,R)
						case RIGHT:
							writeWindow(tr->symbol, encodeStateId(stateId(tr->state, prop)), perm_ptr->permutation[2]);	
						break;

						//Acc and rej, See if there are transitions (q[0],s[1])->ACCEPT/REJECT
						case ACCEPT:
							writeWindow(encodeStateId(acc_state_id), perm_ptr->permutation[1], perm_ptr->permutation[2]);
						break;
						case REJECT:
							writeWindow(encodeStateId(rej_state_id), perm_ptr->permutation[1], perm_ptr->permutation[2]);
						break;
						default:
						break;
					}

				//State is in the center
				}else if(isStateWindow(perm_ptr->permutation[1])){
					tr = delta(prop->states[decodeStateId(perm_ptr->permutation[1])], perm_ptr->permutation[2]);
					switch(tr->move){
						//State moves from center to left (added exception for delimiter '#')
					    //See if there are transitions (q[1],s[2])->(q,s,L)
						case LEFT:
							if(perm_ptr->permutation[0] != '#'){ //can't move to the left if there's a delimiter
								writeWindow(encodeStateId(stateId(tr->state, prop)), perm_ptr->permutation[0], tr->symbol);	
							} 
						break;

						//State moves from center to right 
						//See if there are transitions (q[1],s[2])->(q,s,R)
						case RIGHT:
							writeWindow(perm_ptr->permutation[0], tr->symbol, encodeStateId(stateId(tr->state, prop)));	
						break;

						//Acc and rej, See if there are transitions (q[1],s[2])->ACCEPT/REJECT
						case ACCEPT:
							writeWindow(perm_ptr->permutation[0], encodeStateId(acc_state_id), perm_ptr->permutation[2]);
						break;
						case REJECT:
							writeWindow(perm_ptr->permutation[0], encodeStateId(rej_state_id), perm_ptr->permutation[2]);
						break;
						default:
						break;
					}

				//State is in the right side	
				}else{	
					for(j = 0; j < prop->alphabet_length; ++j){	
						tr = delta(prop->states[decodeStateId(perm_ptr->permutation[2])], prop->alphabet[j]);
						switch(tr->move){
							//State moves from right to center
							//See if there are transitions (q[2],_)->(q,_,L)
							case LEFT:
								writeWindow(perm_ptr->permutation[0], encodeStateId(stateId(tr->state, prop)), perm_ptr->permutation[1]);
							break;

							//State disappears to the right, written symbol appears from the right
							//See if there are transitions (q[2],_)->(_,s,R) 
							case RIGHT:
								writeWindow(perm_ptr->permutation[0], perm_ptr->permutation[1], tr->symbol);
							break;

							//Acc and rej, See if there are transitions (q[2],_)->ACCEPT/REJECT
							case ACCEPT:
								writeWindow(perm_ptr->permutation[0], perm_ptr->permutation[1], encodeStateId(acc_state_id));
							break;
							case REJECT:
								writeWindow(perm_ptr->permutation[0], perm_ptr->permutation[1], encodeStateId(rej_state_id));
							break;
							default:
							break;
						}
					}
				}
			}
		}	

		perm_ptr = perm_ptr->next;
	}
	
	// printPermutations(permutations);
	listdeallocateperm(permutations);
}

//This will fill a list of all possible permutations in the upper row, excluding illegal permutations
void calculatePermutations(permutation_node * l, tm_properties * prop){
    int cell_sym_len = prop->alphabet_length + prop->states_length;
    bool stateFound = false;

	for(int i = 0; i < cell_sym_len; ++i){
		for(int j = 0; j < cell_sym_len; ++j){
			for(int k = 0; k < cell_sym_len; ++k){

				//first part excludes #s# s#s ##s s##
				//second part excludes duplicate state occurrences
				//third part excludes states next to a right delimiter like _q#
				if(((i != 0 || k != 0) && (j != 0)) 
				    &&
				  ((isState(i) && !isState(j) && !isState(k)) || (!isState(i) && isState(j) && !isState(k)) || 
				   (!isState(i) && !isState(j) && isState(k)) || (!isState(i) && !isState(j) && !isState(k)))
				    &&
				  !(isState(j) && k == 0))
				{
					//In windows state ids are encoded as negative numbers
					l->permutation[0] = (isState(i) ? encodeStateId(i-prop->alphabet_length) : prop->alphabet[i]); 
					l->permutation[1] = (isState(j) ? encodeStateId(j-prop->alphabet_length) : prop->alphabet[j]);
					l->permutation[2] = (isState(k) ? encodeStateId(k-prop->alphabet_length) : prop->alphabet[k]);
					l = addPermutation(l);
				}

			}	
		}
	}
}