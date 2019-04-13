#include "tm_lib.h"

//List
char_node * add_char(char_node * l, char e){
    char_node * new = malloc(sizeof(char_node));
    new->elem = e;
    
    while(l->next != NULL){
        l = l->next;
    }
    l->next = new;
    new->prev = l;
    new->next = NULL;

    return new;
}

window_node * add_window(window_node * last){ //window array will be filled manually
    window_node * new = malloc(sizeof(window_node));
    last->next = new;
    new->next = NULL;

    return new; //this will be the new last
}

permutation_node * add_permutation(permutation_node * last){
    permutation_node * new = malloc(sizeof(permutation_node));
    last->next = new;
    new->next = NULL;

    return new;
}

int list_len_char(char_node * l){
    int r;
    if(l != NULL){
        r = 1;
        while(l->next != NULL){
            l = l->next;
            r++;
        }
    }else{
        r = 0;
    }
    return r;
}

int list_len_window(window_node * l){
    int r;
    if(l != NULL){
        r = 1;
        while(l->next != NULL){
            l = l->next;
            r++;
        }
    }else{
        r = 0;
    }
    return r;
}

void tape_to_str(char_node * l, char * str){
    while(l->next != NULL){
        *str = l->elem;
        str++;
        l = l->next;
    }
    *str = l->elem;
    str++;
    *str = '\0';
}

char * tape_to_str_new(char_node * l){
    int n = list_len_char(l);
    char * string = malloc((n+1)*sizeof(char));
    char * r = string;
    while(l->next != NULL){
        *string = l->elem;
        string++;
        l = l->next;
    }
    *string = l->elem;
    string++;
    *string = '\0';

    return r;
}

//Prints
void print_chars(char_node * l){
    printf("%c", l->elem);
    while(l->next != NULL){
        l = l->next;
        printf("%c", l->elem);
    }
    printf("\n");
}

void print_windows(window_node * l){
    int i = 1;
    printf("Window #0\n");
    print_single_window(l->window);
    while(l->next != NULL){
        l = l->next;
        printf("Window #%d\n", i);
        print_single_window(l->window);
        i++;
    }
    printf("\n");
}

void print_single_window(int * w){
    for(int i = 0; i < 6; ++i){
        if(w[i] < 0)
            printf("%d ", decodeStateId(w[i])); //it's a state
        else
            printf("%c ", w[i]); //it's a symbol

        if(i == 2)
            printf("\n");
    }
    printf("\n");
}

void print_permutations(permutation_node * l){
    int i = 1;
    printf("Permutation #0\n");
    print_single_permutation(l->permutation);
    while(l->next != NULL){
        l = l->next;
        printf("Permutation #%d\n", i);
        print_single_permutation(l->permutation);
        i++;
    }
    printf("\n");
}

void print_single_permutation(int * p){
    for(int i = 0; i < 3; ++i){
        if(p[i] < 0)
            printf("%d ", decodeStateId(p[i])); //it's a state
        else
            printf("%c ", p[i]); //it's a symbol
    }
    printf("\n");
}

void print_properties(tm_properties * prop){
    printf("Turing machine data:\n");
    printf("Alphabet: %s\n", prop->alphabet);
    printf("Input string: %s\n", prop->input_string);
    printf("States: ");
    print_array(prop->states, prop->states_length);
    printf("Alphabet length: %d\n", prop->alphabet_length);
    printf("States length: %d\n", prop->states_length);
    printf("Total steps: %d\n\n", prop->tot_steps);
}

void print_array(int * a, int len){
    for(int i = 0; i < len; ++i){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void print_tape(char_node * tape, int head_offset){
    for(int i = 0; i < head_offset; ++i)
        printf(" ");
    printf("V\n");
    print_chars(tape);
    printf("\n");
}

void print_transition(int state, char symbol, transition * tr){
    char strmove[7];
    enum_to_string(strmove, tr->move);
    if(tr->move == ACCEPT || tr->move == REJECT || tr->move == ERROR)
        printf("Transition: %d, %c -> %s\n\n", state, symbol, strmove);
    else
        printf("Transition: %d, %c -> %d, %c, %s\n\n", state, symbol, tr->state, tr->symbol, strmove);
}

//File manipulation
int count_chars(FILE * f){
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

int count_lines(FILE * f){
    char c;
    int r = 1;
    c = fgetc(f);
    while(c != EOF){
        if(c == '\n')
            r++;
        c = fgetc(f);
    }
    rewind(f);

    return r;   
}

int read_int(FILE * f, char * dest){
    int i;
    char c = fgetc(f);
    for(i = 0; i < MAX_INT_DIGITS && isdigit(c); ++i){ 
        dest[i] = c;
        c = fgetc(f);
    }
    dest[i] = '\0';

    return strlen(dest);
}

void write_int(FILE * f, int n){
    char str[MAX_INT_DIGITS+1];
    sprintf(str, "%d", n);
    fprintf(f, "%s", str);
}

void write_long(FILE * f, long n){
    char str[MAX_LONG_DIGITS+1];
    sprintf(str, "%li", n);
    fprintf(f, "%s", str);
}

//Deallocators
void list_dealloc_char(char_node * l){
    while(l->next != NULL){
        l = l->next;
        free(l->prev);
    }
    free(l);
}

void list_dealloc_permutations(permutation_node * l){ 
    if(l->next != NULL){
        list_dealloc_permutations(l->next);
        free(l);
    }else{
        free(l);
    }
}

void list_dealloc_windows(window_node * l){ 
    if(l->next != NULL){
        list_dealloc_windows(l->next);
        free(l);
    }else{
        free(l);
    }
}

//Utils
char_node * blank_node(char_node * prev){
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

char * enum_to_string(char * str, enum action a){
    switch(a){
        case 0:
            strcpy(str, "LEFT");
        break;
        case 1:
            strcpy(str, "RIGHT");
        break;
        case 2:
            strcpy(str, "ACCEPT");
        break;
        case 3:
            strcpy(str, "REJECT");
        break;
        case 4:
            strcpy(str, "ERROR");
        break;
    }
} 

int count_digits_int(int n){
    int r = 0;
    while(n > 0){
        n /= 10;
        ++r;
    }   

    return r;
}

int count_digits_long(long n){
    int r = 0;
    while(n > 0){
        n /= 10;
        ++r;
    }   

    return r;
}