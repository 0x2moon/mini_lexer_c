FLAGS = -ggdb -Wall -Werror -Wfatal-errors
all:
	gcc main.c libs/gstr.c libs/automaton.c libs/lexer.c libs/symbol_table.c -o main $(FLAGS) 

c:
	gcc -g main.c libs/gstr.c libs/automaton.c libs/lexer.c libs/symbol_table.c -o main
	
clean:
	rm -rf  ./main
