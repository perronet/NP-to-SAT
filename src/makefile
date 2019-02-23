default : projectFiles
	gcc -o run.out tm_lib.c parse_dfa.c -I.

#You can specify arguments from here, e.g.: make run ARGS="-s 800 -t"
run : projectFiles
	gcc -o run.out tm_lib.c parse_dfa.c -I.
	./run.out $(ARGS) 

projectFiles : tm_lib.c parse_dfa.c