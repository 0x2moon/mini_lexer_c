#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct symbol_table_list
{
    int number_elements;
    struct symbol_table *start_symbol_table;
    struct symbol_table *final_symbol_table;
};

struct symbol_table
{
    int id;
    char *lexema;
    struct symbol_table *next;
};

struct symbol_table_list *create_st_list();
struct symbol_table *create_st(struct symbol_table_list *stl, int id, char *lexema);
#endif
