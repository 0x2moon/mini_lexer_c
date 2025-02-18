#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../includes/automaton.h"
#include "../includes/gstr.h"

struct automaton *create_automaton()
{
  struct automaton *new = (struct automaton *)malloc(sizeof(struct automaton));
  new->number_state = 0;
  new->initial_state = NULL;
  new->final_state = NULL;
  new->initial_transition = NULL;
  new->final_transition = NULL;
  return new;
}

struct state *create_state(char *name, bool fn_s, bool in_s)
{
  struct state *new = (struct state *)malloc(sizeof(struct state));
  new->final_state = fn_s;
  new->initial_state = in_s;
  new->name = gstraloc(name);
  new->next = NULL;
  return new;
}

struct transition *create_transition(struct state *host_s, struct state *connect_s, char value, bool edhas)
{
  struct transition *new = (struct transition *)malloc(sizeof(struct transition));
  new->host = host_s;
  new->connect = connect_s;
  new->next = NULL;
  if (edhas == false)
  {
    new->start_value = create_value(value);
  }
  return new;
}

struct value *create_value(char value)
{
  struct value *new = (struct value *)malloc(sizeof(struct value));
  new->value_transition = value;
  new->next = NULL;
  return new;
}

struct state *searche_state_in_automaton(struct automaton *automaton, char *name)
{
  struct state *aux = automaton->initial_state;
  if (aux == NULL)
  {
    return NULL;
  }

  while (aux != NULL)
  {
    if (gstrcomp(aux->name, name) == true)
    {
      return aux;
    }
    aux = aux->next;
  }

  return NULL;
}

struct state *insert_state_in_automaton(struct automaton *automaton, struct state *state, char *name, bool end, bool init)
{

  if (automaton->initial_state == NULL)
  {
    automaton->initial_state = create_state(name, end, init);
    automaton->final_state = automaton->initial_state;
    automaton->number_state++;
    return automaton->initial_state;
  }
  automaton->final_state->next = create_state(name, end, init);
  automaton->final_state = automaton->final_state->next;
  automaton->number_state++;
  return automaton->final_state;
}

struct transition *insert_new_transition_with_value_in_automaton(struct automaton *automaton, struct state *host, struct state *connect, char value, bool edhas)
{

  if (automaton->initial_transition == NULL)
  {
    automaton->initial_transition = create_transition(host, connect, value, edhas);
    automaton->final_transition = automaton->initial_transition;
  }
  else
  {
    automaton->final_transition->next = create_transition(host, connect, value, edhas);
    automaton->final_transition = automaton->final_transition->next;
  }
  return automaton->final_transition;
}

void set_new_information_in_state(struct state *state, bool final_state, bool init_state)
{
  if ((state->initial_state != true) && (init_state == true))
  {
    state->initial_state = init_state;
  }
  state->final_state = final_state;
}

void insert_value_on_transition_with(struct transition *transition, char value)
{
  struct value *aux = transition->start_value;
  while (aux->next != NULL)
  {
    aux = aux->next;
  }
  aux->next = create_value(value);
  return;
}

struct transition *searche_transition_in_automaton(struct transition *transition, struct state *host, struct state *connect)
{
  struct transition *aux = transition;
  while (aux != NULL)
  {
    if ((aux->host == host) && (aux->connect == connect))
    {
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

void insert_value_on_transition_with(struct transition *transition, char value);

struct automaton *the_configuration_file_into_the_automaton(struct automaton *automaton_rules_lenguagem, char *config_file)
{
  FILE *input_file = NULL;
  input_file = fopen(config_file, "r");
  if (input_file == NULL)
  {
    printf("[ERROR] config file error");
    exit(1);
  }

  char buffer, buffer_transition, *aux = NULL;
  int arrow_control = 0, comma_control = 0, size = 1;
  bool init = false, end = false, end_delimiter_hyphen_automaton_syntax = false;

  struct state *host_state_line_read = NULL;
  struct state *connected_state_line_read = NULL;
  struct transition *transition_line = NULL;

  while (fread(&buffer, sizeof(char), 1, input_file))
  {
    if ((aux == NULL) && (host_state_line_read == NULL) && (connected_state_line_read == NULL) && (transition_line == NULL) && ((buffer == ' ') || (buffer == '\n')))
    {
      continue;
    }

    if (buffer == ' ')
    {
      continue;
    }

    if (buffer == '*')
    {
      end = true;
      continue;
    }

    if ((buffer == '-') && (comma_control == 1) && (end == true))
    {
      end_delimiter_hyphen_automaton_syntax = true;
      continue;
    }

    if (buffer == '-')
    {
      arrow_control++;
      continue;
    }

    if (buffer == '>')
    {
      if (arrow_control == 1 && buffer == '>')
      {
        init = true;
        arrow_control = 0;
      }
      continue;
    }

    if (buffer == '*')
    {
      end = true;
      continue;
    }

    if (buffer == ',')
    {
      comma_control++;
      if (comma_control == 1)
      {
        host_state_line_read = searche_state_in_automaton(automaton_rules_lenguagem, aux);
        if (host_state_line_read == NULL)
        {
          host_state_line_read = insert_state_in_automaton(automaton_rules_lenguagem, automaton_rules_lenguagem->initial_state, aux, end, init);
        }
        else
        {
          set_new_information_in_state(host_state_line_read, end, init);
        }
      }
      if (comma_control == 2)
      {
        comma_control = 0;
        if (aux == NULL)
        {
          if ((end == true) && (buffer_transition == '-'))
          {
            end_delimiter_hyphen_automaton_syntax = true;
          }
        }
        else
        {
          if (gstrlen(aux) > 1)
          {
            if (gstrcomp(aux, "2D"))
            {
              buffer_transition = '-';
            }

            if (gstrcomp(aux, "2C"))
            {
              buffer_transition = ',';
            }

            if (gstrcomp(aux, "20"))
            {
              buffer_transition = 0x20;
            }

             if (gstrcomp(aux, "5F"))
            {
              buffer_transition = 0x5F;
            }

            if (gstrcomp(aux, "2F"))
            {
              buffer_transition = '/';
            }
            if (gstrcomp(aux, "0A"))
            {
              buffer_transition = 0xA;
            }
            if (gstrcomp(aux, "3E"))
            {
              buffer_transition = 0x3E;
            }
             if (gstrcomp(aux, "3C"))
            {
              buffer_transition = 0x3C;
            }
             if (gstrcomp(aux, "7C"))
            {
              buffer_transition = 0x7C;
            }
              if (gstrcomp(aux, "2A"))
            {
              buffer_transition = 0x2A;
            }
             if (gstrcomp(aux, "26"))
            {
              buffer_transition = 0x26;
            }
          }
          else
          {
            buffer_transition = *aux;
          }
        }
      }

      free(aux);
      aux = NULL;
      continue;
    }

    if (buffer == '\n')
    {
      if (end_delimiter_hyphen_automaton_syntax == true)
      {
        transition_line = insert_new_transition_with_value_in_automaton(automaton_rules_lenguagem, host_state_line_read, NULL, '\0', end_delimiter_hyphen_automaton_syntax);
        free(aux);
        aux = NULL;
        connected_state_line_read = NULL;
        transition_line = NULL;
        host_state_line_read = NULL;
        init = false;
        end = false;
        end_delimiter_hyphen_automaton_syntax = false;
        continue;
      }
      else
      {
        connected_state_line_read = searche_state_in_automaton(automaton_rules_lenguagem, aux);
        if ((connected_state_line_read == NULL) && (aux != NULL))
        {
          connected_state_line_read = insert_state_in_automaton(automaton_rules_lenguagem, automaton_rules_lenguagem->initial_state, aux, end, init);
        }
        transition_line = searche_transition_in_automaton(automaton_rules_lenguagem->initial_transition, host_state_line_read, connected_state_line_read);
        if (transition_line == NULL)
        {
          transition_line = insert_new_transition_with_value_in_automaton(automaton_rules_lenguagem, host_state_line_read, connected_state_line_read, buffer_transition, end_delimiter_hyphen_automaton_syntax);
        }
        else
        {
          insert_value_on_transition_with(transition_line, buffer_transition);
        }
        free(aux);
        aux = NULL;
        connected_state_line_read = NULL;
        transition_line = NULL;
        host_state_line_read = NULL;
        init = false;
        end = false;
        end_delimiter_hyphen_automaton_syntax = false;
        continue;
      }
    }
    if (aux == NULL)
    {
      size = 1;
      aux = (char *)calloc((size), sizeof(char));
      gstrsetvalue(&aux, (size - 1), buffer);
      size++;
    }
    else
    {
      aux = realloc(aux, (size + 1) * sizeof(char));
      gstrsetvalue(&aux, (size - 1), buffer);
      size++;
    }
  }
  return automaton_rules_lenguagem;
}

struct all_automaton *create_all()
{
  struct all_automaton *new = (struct all_automaton *)malloc(sizeof(struct all_automaton));
  new->files_reads = 0;
  new->initial_automaton = NULL;
  new->final_automaton = NULL;
  return new;
}

struct automaton_node *create_node_automaton(char *path)
{
  struct automaton_node *new = (struct automaton_node *)malloc(sizeof(struct automaton_node));
  new->name = gstraloc(path);
  new->current = NULL;
  new->next = NULL;
  return new;
}

struct automaton_node *insert_new_automaton_in_node_automaton(struct all_automaton *all, struct automaton_node *node, char *path_name)
{
  char fullpath[80];
  sprintf(fullpath, "files/automaton_syntax_file/%s", path_name);
  ensure_zeroA_end_file(fullpath);
  if (all->initial_automaton == NULL)
  {
    all->initial_automaton = create_node_automaton(path_name);
    all->initial_automaton->current = create_automaton();
    all->initial_automaton->current = the_configuration_file_into_the_automaton(all->initial_automaton->current, fullpath);
    all->final_automaton = all->initial_automaton;
  }
  else
  {
    node->next = create_node_automaton(path_name);
    all->final_automaton = node->next;
    all->final_automaton->current = create_automaton();
    all->final_automaton->current = the_configuration_file_into_the_automaton(all->final_automaton->current, fullpath);
  }
  all->files_reads++;
  return all->final_automaton;
}
