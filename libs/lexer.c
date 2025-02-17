#include "../includes/symbol_table.h"
#include "../includes/automaton.h"
#include "../includes/lexer.h"
#include "../includes/gstr.h"
#include <stdlib.h>
#include <stdio.h>

int control = 0, Line = 0, Colunm = 0;
bool token_insert = false;
char buffer = '\0', especial_buffer = '\0';

struct list_tokens *create_list_token()
{
    struct list_tokens *new = (struct list_tokens *)malloc(sizeof(struct list_tokens));
    new->number_tokens = 0;
    new->final_token = NULL;
    new->initial_token = NULL;
    return new;
}

struct token *create_token(struct list_tokens *lt, int line, int columns, char *kw, char *lex)
{
    struct token *new = (struct token *)malloc(sizeof(struct token));
    lt->number_tokens++;
    new->id = lt->number_tokens;
    new->columns = columns;
    new->line = line;
    new->keyword = gstraloc(kw);
    new->lexema = gstraloc(lex);
    return new;
}

void insert_token_in_list_of_token(struct list_tokens *lt, int column, int line, char *kw, char *pathname)
{
    if (lt->initial_token == NULL)
    {
        lt->initial_token = create_token(lt, line, column, kw, pathname);
        lt->final_token = lt->initial_token;
    }
    else
    {
        lt->final_token->next = create_token(lt, line, column, kw, pathname);
        lt->final_token = lt->final_token->next;
    }
    return;
}

FILE *open_file_c_for_read_automaton(char *file_c)
{
    FILE *new = NULL;
    new = fopen(file_c, "r");
    if (new == NULL)
    {
        printf("[ERROR] Open c file");
        exit(1);
    }
    return new;
}

void control_read_file_in_c_and_make_token_fill_symbol_table_and_token_list(struct all_automaton *all, struct list_tokens *ltk, struct symbol_table_list *stb, char *path)
{
    int *Line_current, *Colunms_current;
    control = 0;
    Line_current = (int *)malloc(sizeof(int));
    Colunms_current = (int *)malloc(sizeof(int));
    *Line_current = 0;
    *Colunms_current = 0;
    char *character_sequence = NULL;
    character_sequence = (char *)calloc(1, sizeof(char));
    gstrsetvalue(&character_sequence, 0, '\0');
    FILE *analyser_file = open_file_c_for_read_automaton(path);
    struct automaton_node *current_auto = all->initial_automaton;
    int i = 0;

    while (i < all->files_reads)
    {
        if (feof(analyser_file))
        {
            break;
            fclose(analyser_file);
        }

        printf("[ %s ] - > [ %s ] \n", character_sequence, current_auto->name);
        character_sequence = read_file_in_c_and_make_token_fill_symbol_table_and_token_list(analyser_file, all, current_auto->current, stb, ltk, current_auto->name, character_sequence, Line_current, Colunms_current, buffer, especial_buffer);
        if (control == 1)
        {
            i = 0;
            control = 0;
            current_auto = all->initial_automaton;
            continue;
        }
        current_auto = current_auto->next;
        i++;
    }
}

struct transition *take_init_state(struct automaton *automaton)
{
    struct transition *aux = automaton->initial_transition;
    while (aux != NULL)
    {
        if (aux->host->initial_state == true)
        {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

struct transition *next_transition_sequence(struct transition *transition, char *next_transiiton)
{
    while (transition->next != NULL)
    {
        if (gstrcomp(transition->host->name, next_transiiton) == true)
        {
            return transition;
        }
        transition = transition->next;
    }
    return NULL;
}

struct transition *search_connect_state_in_automaton(struct transition *transition, char *state)
{
    while (transition->next != NULL)
    {
        if (state == NULL)
        {
            return NULL;
        }

        if (gstrcomp(transition->host->name, state) == true)
        {
            return transition;
        }
        transition = transition->next;
    }
    return transition;
}

bool checkline(struct automaton *automaton, char *buffer, int size)
{
    struct transition *transition = take_init_state(automaton);
    struct value *aux = transition->start_value;
    bool check = false;
    int i = 0;

    if (size == 1)
    {
        check = gcharcomp(*buffer, aux->value_transition);
    }
    else
    {
        while (buffer[i] != '\0')
        {
            check = gcharcomp(buffer[i], aux->value_transition);
            if (check == true)
            {
                transition = search_connect_state_in_automaton(automaton->initial_transition, transition->connect->name);
                if (i == size - 1)
                {
                    break;
                }
                else
                {
                    aux = transition->start_value;
                    i++;
                    continue;
                }
            }
            else
            {
                if (aux->next == NULL)
                {
                    return false;
                }
                aux = aux->next;
            }
        }
    }
    if ((check == true) && (transition->host->final_state))
    {
        return true;
    }
    return false;
}

int is_delimiter(char c)
{
    char delimiter[] = ",(){}[];+-*/%&|^~!=<> \n";
    for (int i = 0; delimiter[i] != '\0'; i++)
    {
        if (gcharcomp(c, delimiter[i]) == true)
        {
            return 1;
        }
    }
    return 0;
}

char *read_file_in_c_and_make_token_fill_symbol_table_and_token_list(FILE *file, struct all_automaton *all, struct automaton *automaton, struct symbol_table_list *stb, struct list_tokens *ltk, char *automaton_name, char *character_sequence, int *Line_current, int *Colunms_current, char buffer, char especial_buffer)
{
    int size = 0;
    if (token_insert == true)
    {
        if ((especial_buffer != '\0') && (especial_buffer != '\n') && (especial_buffer != ' '))
        {
            insert_delimiter_token_list(all, *Line_current, *Colunms_current, ltk, especial_buffer);
            especial_buffer = '\0';
        }
        token_insert = false;
    }

    if (is_empty(character_sequence) == true)
    {
        buffer = '\0';
        especial_buffer = '\0';
        size = 2;
        fread(&buffer, sizeof(char), 1, file);
        if (is_delimiter(buffer) == 1)
        {
            especial_buffer = buffer;
            if ((especial_buffer != '\0') && (especial_buffer != '\n') && (especial_buffer != ' '))
            {
                insert_delimiter_token_list(all, *Line_current, *Colunms_current, ltk, especial_buffer);
            }
            especial_buffer = '\0';
            buffer = '\0';
        }
        else
        {
            character_sequence = realloc(character_sequence, (size + 1) * sizeof(char));
            gmemset(&character_sequence, gstrlen(character_sequence), '\0');
            gstrsetvalue(&character_sequence, (size - 2), buffer);
            size++;
        }
        Colunm++;

        while (fread(&buffer, sizeof(char), 1, file) != EOF)
        {
            if (buffer == '\n')
            {
                Colunm = 0;
                Line++;
                if (is_empty(character_sequence) == true)
                {
                    continue;
                }
                break;
            }

            if (buffer == ' ')
            {
                if (is_empty(character_sequence) == true)
                {
                    continue;
                }
                break;
            }

            if (is_delimiter(buffer) == 1)
            {
                especial_buffer = buffer;
                break;
            }
            character_sequence = realloc(character_sequence, (size + 1) * sizeof(char));
            gstrsetvalue(&character_sequence, (size - 2), buffer);
            size++;
        }
        character_sequence[size - 1] = '\0';
        *Colunms_current = Colunm;
        *Line_current = Line;
    }
    bool check = checkline(automaton, character_sequence, gstrlen(character_sequence));
    if (check == true)
    {
        printf("\n[INSERIDO]-> [ %s ] char -> [ %c ], -> [ %s ]\n", character_sequence, especial_buffer, automaton_name);
        if (gstrcomp(automaton_name, "IDENTIFIERS") == true)
        {
            insert_indentifier_in_symbol_table(stb, (stb->number_elements + 1), character_sequence);
        }
        insert_token_in_token_list(ltk, *Line_current, *Colunms_current, character_sequence, automaton_name);
        control = 1;
        gmemset(&character_sequence, gstrlen(character_sequence), '\0');
        token_insert = true;
        // gstrsetvalue(&character_sequence, 0, '\0');
    }
    return character_sequence;
}

void insert_token_in_token_list(struct list_tokens *ltk, int line, int columns, char *kw, char *lex)
{
    if (ltk->initial_token == NULL)
    {
        ltk->initial_token = create_token(ltk, line, columns, kw, lex);
        ltk->final_token = ltk->initial_token;
    }
    else
    {
        ltk->final_token->next = create_token(ltk, line, columns, kw, lex);
        ltk->final_token = ltk->final_token->next;
    }
    return;
}

void insert_indentifier_in_symbol_table(struct symbol_table_list *stb, int id, char *lexema)
{
    if (stb->start_symbol_table == NULL)
    {
        stb->start_symbol_table = create_st(stb, id, lexema);
        stb->final_symbol_table = stb->start_symbol_table;
    }
    else
    {
        stb->final_symbol_table->next = create_st(stb, id, lexema);
        stb->final_symbol_table = stb->final_symbol_table->next;
    }
    return;
}

void insert_delimiter_token_list(struct all_automaton *all, int line, int column, struct list_tokens *ltk, char delimiter)
{
    struct automaton_node *current_auto = all->initial_automaton;
    for (int i = 0; i < all->files_reads; i++)
    {
        bool check = checkline(current_auto->current, &delimiter, 1);
        if (check == true)
        {
            insert_token_in_list_of_token(ltk, column + 1, line + 1, &delimiter, current_auto->name);
            return;
        }
        current_auto = current_auto->next;
    }
    return;
}
