#include "tm_lib.h"

#define HEADER "include\"tm_lib.h\"\ntransition * delta(int state, char symbol){\ntransition * t;\nswitch(state){\n"

int main(int argc, char const *argv[])
{
	FILE * input = fopen("input_program", "r");
	FILE * output = fopen("delta.c", "w");
	char c;
	bool wasComment, nextState;
	char * str_state = malloc(6*sizeof(char));
	char * str_acc = malloc(7*sizeof(char));
	char * str_arrow = malloc(5*sizeof(char));



	fprintf(output, HEADER);

	while((c = fgetc(input)) != EOF){
		wasComment = false;

		switch(c){
			case '/':
				c = fgetc(input);
				if(c == '/'){
					wasComment = true;
					while(c != '\n' && c != EOF){
						c = fgetc(input);
					}
				}else{
					fprintf(output, "/%c", c);
				}
			break;

			case 's':
				if(!strcmp(fgets(str_state, 6, input), "tate ")){
					c = fgetc(input);
					printf("Found state %d\n", c);
					fprintf(output, "case %d:\n", c);
					while(c != EOF && c != '\n'){
						c = fgetc(input);
					}
					c = fgetc(input); //c will now be the input symbol

					fprintf(output, "switch(symbol){\n");
					fprintf(output, "case '%c':\n", c);

					//read arrow and output of the transition
					if(!strcmp(fgets(str_arrow, 5, input), " -> ")){
						str_acc = fgets(str_acc, 7, input);
						if(!strcmp(str_acc, "ACCEPT")){
							fprintf(output, "t.move = ACCEPT;\n");
						}else if(!strcmp(str_acc, "REJECT")){
							fprintf(output, "t.move = REJECT;\n");
						}else{
							//TODO read actual input a, b, c. may ignore commas
						}
					}else{
						//ERROR
					}

					fprintf(output, "break;\n");
				}else{
					fprintf(output, "%c", c);
				}

			break;
		}

		if(!wasComment){
			fprintf(output, "%c", c);
		}
	}
	fprintf(output, "\n}\nreturn t;\n}");
	fclose(input);
	fclose(output);
	return 0;
}