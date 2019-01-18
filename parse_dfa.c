#include <stdio.h>
typedef char bool;

int main(int argc, char const *argv[])
{
	FILE * dfa = fopen("./dfa.txt", "r");
	printf("hello world, %d\n", fgetc(dfa));
	return 0;
}