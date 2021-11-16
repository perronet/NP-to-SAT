all: projectFiles
	gcc -o src/run.out src/tm_lib.c src/parse_dfa.c -Isrc

pp: projectFiles
	gcc src/tm_lib.c src/parse_dfa.c -Isrc -E

#You can specify arguments from here, e.g.: make run ARGS="-s 800 -t"
run: projectFiles
	gcc -o run.out src/tm_lib.c src/parse_dfa.c -Isrc
	./run.out $(ARGS) 

projectFiles : src/tm_lib.c src/parse_dfa.c

clean:
	rm src/run.out
