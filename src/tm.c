#ifdef DELTA
#include "delta.h"
#endif
#include "tm_lib.h"

int main(int argc, char const *argv[]){
	printf("Hi\n");
	transition * r = delta(0, 'a');
	printf("%d\n", r->state);
	printf("%c\n", r->symbol);
	printf("%d\n", r->move);
	return 0;
}