#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
struct token
{
    int id, line, columns;
    char *keyword;
    char *lexema;
    struct token *next;
};

struct list_tokens
{
    int number_tokens;
    struct token *initial_token;
    struct token *final_token;
};

struct list_tokens *create_list_token();
struct token *create_token(struct list_tokens *lt, int line, int columns, char *kw, char *lex);
void insert_token_in_list_of_token(struct list_tokens *lt, int column, int line, char *kw, char *pathname);
void insert_delimiter_token_list(struct all_automaton *all, int line, int column, struct list_tokens *ltk, char delimiter);
char *read_file_in_c_and_make_token_fill_symbol_table_and_token_list(FILE *file, struct all_automaton *all, struct automaton *automaton, struct symbol_table_list *stb, struct list_tokens *ltk, char *automaton_name, char *character_sequence, int *Line_current, int *Colunms_current, char buffer, char especial_buffer);
void control_read_file_in_c_and_make_token_fill_symbol_table_and_token_list(struct all_automaton *all, struct list_tokens *ltk, struct symbol_table_list *stb, char *path);
void insert_token_in_token_list(struct list_tokens *ltk, int line, int columns, char *kw, char *lex);
void insert_indentifier_in_symbol_table(struct symbol_table_list *stb, int id, char *lexema);

bool verify_file_with_automaton(struct all_automaton *all, char *character_sequence, struct symbol_table_list *stb, struct list_tokens *ltk);
bool checkline(struct automaton *automaton, char *buffer, int size);
#endif