#ifndef AUTOMATON_H
#include <stdbool.h>

struct state
{
  char *name;
  bool final_state;
  bool initial_state;
  struct state *next;
};

struct transition
{
  struct state *host;
  struct state *connect;
  struct value *start_value;
  struct transition *next;
};

struct value
{
  char value_transition;
  struct value *next;
};

struct automaton
{
  unsigned int number_state;
  struct state *initial_state;
  struct state *final_state;
  struct transition *initial_transition;
  struct transition *final_transition;
};

struct all_automaton
{
  int files_reads;
  struct automaton_node *initial_automaton;
  struct automaton_node *final_automaton;
};

struct automaton_node
{
  char *name;
  struct automaton *current;
  struct automaton_node *next;
};

struct automaton *create_automaton();
struct transition *create_transition(struct state *, struct state *, char, bool);
struct state *create_state(char *, bool, bool);
struct value *create_value(char);
struct all_automaton *create_all();
struct automaton_node *create_node_automaton(char *);

struct automaton *the_configuration_file_into_the_automaton(struct automaton *automaton_rules_lenguagem, char *config_file);
struct state *searche_state_in_automaton(struct automaton *automaton, char *);
struct state *insert_state_in_automaton(struct automaton *automaton, struct state *state, char *, bool end, bool init);
struct transition *insert_new_transition_with_value_in_automaton(struct automaton *, struct state *, struct state *, char, bool);
struct transition *searche_transition_in_automaton(struct transition *transition, struct state *host, struct state *connect);
void insert_value_on_transition_with(struct transition *transition, char value);

void insert_new_automaton_in_the_all_list_of_automaton(struct all_automaton *, char *);
struct automaton_node *insert_new_automaton_in_node_automaton(struct all_automaton *all, struct automaton_node *node, char *path_name);
#endif
