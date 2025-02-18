#include "../includes/symbol_table.h"
#include "../includes/automaton.h"
#include "../includes/lexer.h"
#include "../includes/gstr.h"
#include <stdlib.h>
#include <stdio.h>

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
        printf("[ERROR] Open c file (%s)\n", file_c);
        exit(1);
    }
    return new;
}

bool is_delimiter(char c)
{
    char delimiter[] = ",(){}[];+-*/%&|^~!>=< \n";
    for (int i = 0; delimiter[i] != '\0'; i++)
    {
        if (gcharcomp(c, delimiter[i]) == true)
        {
            return true;
        }
    }
    return false;
}

void control_read_file_in_c_and_make_token_fill_symbol_table_and_token_list(struct all_automaton *all, struct list_tokens *ltk, struct symbol_table_list *stb, char *path)
{
    int line = 0, colunm = 0, size = 1;
    char *character_sequence = NULL, *duplo = NULL;
    char buffer = '\0';
    duplo = (char *)calloc(3, sizeof(char));
    gmemset(&duplo, 3, '\0');
    character_sequence = (char *)calloc(size, sizeof(char));
    gmemset(&character_sequence, gstrlen(character_sequence), '\0');
    gstrsetvalue(&character_sequence, 0, '\0');
    size++;

    FILE *analyser_file = open_file_c_for_read_automaton(path);

    while (fread(&buffer, sizeof(char), 1, analyser_file))
    {
        if (feof(analyser_file))
        {
            printf("final de arquivo");
            break;
        }
        colunm++;
        if (is_delimiter(buffer) == true)
        {
            if ((buffer == '\n') || (buffer == ' '))
            {
                if (buffer == '\n')
                {
                    colunm = 0;
                    line++;
                }
            }
            else
            {
                duplo[0] = buffer;
                fread(&buffer, sizeof(char), 1, analyser_file);
                if (is_delimiter(buffer) == true)
                {
                    duplo[1] = buffer;
                    if ((buffer == '>') || (buffer == '|') || (buffer == '=') || (buffer == '<') || (buffer == '&') || (buffer == '+'))
                    {
                        if (verify_file_with_automaton(all, duplo, stb, ltk, line, colunm) == false)
                        {
                            gcolored_print("\n[ERROR] ->", RED);
                            printf("linha %d coluna %d [ %s ]", line, colunm, character_sequence);
                        }

                        continue;
                    }
                    else
                    {
                        fseek(analyser_file, -1, SEEK_CUR);
                        buffer = duplo[0];
                    }
                }
                else
                {
                    fseek(analyser_file, -1, SEEK_CUR);
                    buffer = duplo[0];
                }
            }

            if (is_empty(character_sequence) == false)
            {
                character_sequence[size - 1] = '\0';
                if (verify_file_with_automaton(all, character_sequence, stb, ltk, line, colunm) == false)
                {
                    gcolored_print("\n[ERROR] ->", RED);
                    printf("linha %d coluna %d [ %s ]\n", line, colunm, character_sequence);
                }
                free(character_sequence);
                size = 1;
                character_sequence = (char *)calloc(size, sizeof(char));
                gmemset(&character_sequence, gstrlen(character_sequence), '\0');
                size++;
            }
            if ((buffer != '\0') && (buffer != '\n') && (buffer != ' '))
            {
                insert_delimiter_token_list(all, line, colunm, ltk, buffer);
                buffer = '\0';
            }
            continue;
        }
        character_sequence = realloc(character_sequence, (size + 1) * sizeof(char));
        gstrsetvalue(&character_sequence, (size - 2), buffer);
        size++;
    }
}

bool verify_file_with_automaton(struct all_automaton *all, char *character_sequence, struct symbol_table_list *stb, struct list_tokens *ltk, int line, int colunm)
{
    struct automaton_node *aux = all->initial_automaton;
    for (int i = 0; i < all->files_reads; i++)
    {
        bool check = checkline(aux->current, character_sequence, gstrlen(character_sequence));
        if (check == true)
        {
            if (gstrcomp(aux->name, "IDENTIFIERS") == true)
            {
                insert_indentifier_in_symbol_table(stb, (stb->number_elements + 1), character_sequence);
            }
            insert_token_in_token_list(ltk, line, colunm, character_sequence, aux->name);
            printf("%s  ", aux->name);
            gmemset(&character_sequence, gstrlen(character_sequence), '\0');
            return true;
        }
        aux = aux->next;
    }
    return false;
}

int size_initial_state_transition(struct automaton *aut, struct state *state)
{
    if (aut == NULL || state == NULL)
        return 0;

    int size = 0;
    struct state *initial_state = state;
    struct transition *aux = aut->initial_transition;

    while (aux != NULL)
    {
        if (aux->host != NULL && gstrcomp(aux->host->name, initial_state->name))
        {
            size++;
        }
        aux = aux->next;
    }

    return size;
}

void transition_next(struct automaton *automaton, struct state *state, struct transition *list[], int size)
{

    if (automaton == NULL || list == NULL || size <= 0)
        return;

    int i = 0;
    for (; i < size; i++)
    {
        list[i] = NULL;
    }

    struct state *initial_state = state;
    if (initial_state == NULL)
        return;

    i = 0;
    struct transition *tr = automaton->initial_transition;

    while (tr != NULL && i < size)
    {
        if (tr->host != NULL && gstrcomp(tr->host->name, initial_state->name))
        {
            list[i] = tr;
            i++;
        }
        tr = tr->next;
    }
}

struct state *take_init_state(struct automaton *automaton)
{
    struct transition *aux = automaton->initial_transition;
    while (aux != NULL)
    {
        if (aux->host->initial_state == true)
        {
            return aux->host;
        }
        aux = aux->next;
    }
    return NULL;
}

struct transition *take_two_init_state(struct transition *aux)
{
    aux = aux->next;
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

bool checkline(struct automaton *automaton, char *buffer, int buf_size)
{

    if (automaton == NULL || buffer == NULL || buf_size <= 0)
        return false;

    int control = size_initial_state_transition(automaton, take_init_state(automaton));
    if (control <= 0)
        return false;

    struct transition *list[10] = {NULL};
    transition_next(automaton, automaton->initial_state, list, control);

    int list_control = 0;
    struct transition *current_transition = list[list_control];
    if (current_transition == NULL)
        return false;

    struct value *current_value = current_transition->start_value;
    bool match_found = false;
    int i = 0;

    if (buf_size == 1)
    {
        while (current_value != NULL)
        {
            match_found = gcharcomp(*buffer, current_value->value_transition);
            if (match_found)
                break;
            current_value = current_value->next;
        }
    }
    else
    {
        while (buffer[i] != '\0')
        {
            if (current_value == NULL)
            {
                if (list_control < control -1)
                {
                    list_control++;
                    current_transition = list[list_control];
                    current_value = current_transition->start_value;
                    continue;
                }
                return false;
            }
            match_found = gcharcomp(buffer[i], current_value->value_transition);
            if (match_found)
            {
                current_transition = search_connect_state_in_automaton(automaton->initial_transition, current_transition->connect->name);
                if (i == buf_size - 1)
                {
                    break;
                }
                else
                {
                    control = size_initial_state_transition(automaton, current_transition->host);
                    for (int j = 0; j < 10; j++)
                    {
                        list[j] = NULL;
                    }
                    transition_next(automaton, current_transition->host, list, control);
                    list_control = 0;
                    current_transition = list[list_control];
                    if (current_transition == NULL)
                        break;

                    current_value = current_transition->start_value;
                    i++;
                    continue;
                }
            }
            else
            {
                if (current_value->next == NULL)
                {
                    if (list_control < control -1)
                    {
                        list_control++;
                        current_transition = list[list_control];
                        current_value = current_transition->start_value;
                        continue;
                    }
                    return false;
                }
                current_value = current_value->next;
            }
        }
    }
    if ((match_found == true) && (current_transition->host->final_state == true))
    {
        return true;
    }
    return false;
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
            printf("%s  ", current_auto->name);
            return;
        }
        current_auto = current_auto->next;
    }
    return;
}

// void print_stb(struct symbol_table_list *st, struct list_tokens *tk)
// {
//     struct symbol_table *aux = st->start_symbol_table;
//     for (int i = 0; i < aux ; i++)
//     {
//         /* code */
//     }

// }