#include "../includes/symbol_table.h"
#include "../includes/gstr.h"
#include <stdlib.h>

struct symbol_table_list *create_st_list()
{
    struct symbol_table_list *new = (struct symbol_table_list *) malloc(sizeof(struct symbol_table_list));
    new->start_symbol_table = NULL;
    new->final_symbol_table = NULL;
    new->number_elements = 0;
    return new;
}

struct symbol_table *create_st(struct symbol_table_list *stl, int id, char *lexema)
{
    struct symbol_table *new = (struct symbol_table *) malloc(sizeof(struct symbol_table_list));
    stl->number_elements++;
    new->id = id;
    new->lexema = gstraloc(lexema);
    new->next = NULL;
    return new;
}
