#include "tm_lib.h"

char_node * listadd(char_node * l, char e){
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

void listprint(char_node * l){
	printf("%c", l->elem);
	while(l->next != NULL){
		l = l->next;
		printf("%c", l->elem);
	}
	printf("\n");
}

void listcpystring(char_node * l, char * str){
	while(l->next != NULL){
		*str = l->elem;
		str++;
		l = l->next;
	}
	*str = l->elem;
	str++;
	*str = '\0';
}

char * listcpystring_new(char_node * l){
	int n = listlength(l);
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

int listlength(char_node * l){
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

void listdeallocate(char_node * l){
	while(l->next != NULL){
		l = l->next;
		free(l->prev);
	}
	free(l);
}

char * enumToString(char * str, enum action a){
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

void printProperties(tm_properties * prop){
	printf("Turing machine data:\n");
	printf("Alphabet: %s\n", prop->alphabet);
	printf("Input string: %s\n", prop->input_string);
	printf("States: ");
	printarray(prop->states, prop->states_length);
	printf("Alphabet length: %d\n", prop->alphabet_length);
	printf("States length: %d\n", prop->states_length);
	printf("Total steps: %d\n", prop->tot_steps);
}

void printarray(int * a, int len){
	for(int i = 0; i < len; ++i){
		printf("%d ", a[i]);
	}
	printf("\n");
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

int countLines(FILE * f){
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