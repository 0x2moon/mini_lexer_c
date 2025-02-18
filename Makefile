FLAGS = -ggdb -Wall -Werror -Wfatal-errors
all:
	gcc main.c libs/gstr.c libs/automaton.c libs/lexer.c libs/symbol_table.c -o main $(FLAGS) 

run:
	gcc -g main.c libs/gstr.c libs/automaton.c libs/lexer.c libs/symbol_table.c -o main
	./main files/utils_files/exemplo.c
clean:
	rm -rf  ./main
