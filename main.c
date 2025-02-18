#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "includes/symbol_table.h"
#include "includes/automaton.h"
#include "includes/lexer.h"
#include "includes/gstr.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Uso incorreto. Por favor, forneca o path do arquivo .c como argumento.\nExemplo:\n");
    gcolored_print("\tfiles/utils_files/exemplo.c\n",GREEN);
    printf("Use make run para executar o arquivo de exemplo\n");
    return 1;
  }

  struct all_automaton *automaton_rules_list = create_all();
  struct list_tokens *tokens_of_input_files = create_list_token();
  struct symbol_table_list *symbol_of_table = create_st_list();
  char *path_analyser_file = gstraloc(argv[1]);
  
  char *filename[] =
      {
          "KEYWORD_AUTO",
          "KEYWORD_BREAK",
          "KEYWORD_CASE",
          "KEYWORD_CHAR",
          "KEYWORD_CONST",
          "KEYWORD_CONTINUE",
          "KEYWORD_DEFAULT",
          "KEYWORD_DOUBLE",
          "KEYWORD_ELSE",
          "KEYWORD_ENUM",
          "KEYWORD_EXTERN",
          "KEYWORD_FLOAT",
          "KEYWORD_FOR",
          "KEYWORD_DO",
          "KEYWORD_GOTO",
          "KEYWORD_IF",
          "KEYWORD_INT",
          "KEYWORD_LONG",
          "KEYWORD_REGISTER",
          "KEYWORD_RETURN",
          "KEYWORD_SHORT",
          "KEYWORD_SIGNED",
          "KEYWORD_SIZEOF",
          "KEYWORD_STATIC",
          "KEYWORD_STRUCT",
          "KEYWORD_SWITCH",
          "KEYWORD_TYPEDEF",
          "KEYWORD_UNION",
          "KEYWORD_UNSIGN",
          "KEYWORD_UNSIGNED",
          "KEYWORD_VOID",
          "KEYWORD_WHILE",
          "KEYWORD_VOLATILE",
          "ARITHMETIC_OPERATORS_DIVISION",
          "ARITHMETIC_OPERATORS_MODULO_OR_REMAINDER_OF_DIVISION",
          "ARITHMETIC_OPERATORS_MULTIPLICATION",
          "BITWISE_OPERATORS_LEFT_SHIFT",
          "BITWISE_OPERATORS_RIGHT_SHIFT",
          "LOGICAL_OPERATORS_AND",
          "LOGICAL_OPERATORS_OR",
          "RELATIONAL_OPERATORS_EQUAL_TO",
          "RELATIONAL_OPERATORS_GREATER_OR_EQUAL_TO",
          "RELATIONAL_OPERATORS_GREATER_THAN",
          "RELATIONAL_OPERATORS_LESS_OR_EQUAL_TO",
          "RELATIONAL_OPERATORS_LESS_THAN",
          "RELATIONAL_OPERATORS_NOT_EQUAL_TO",
          "SEPARATORS_CLOSE_BRACE",
          "SEPARATORS_CLOSE_BRACKET",
          "SEPARATORS_CLOSE_PARENTHESIS",
          "SEPARATORS_COMMA",
          "SEPARATORS_OPEN_BRACE",
          "SEPARATORS_OPEN_BRACKET",
          "SEPARATORS_OPEN_PARENTHESIS",
          "SEPARATORS_SEMICOLON",
          "UNARY_OPERATORS_ADDRESS_OF",
          "UNARY_OPERATORS_BITWISE_NOT",
          "UNARY_OPERATORS_DECREMENT",
          "UNARY_OPERATORS_INCREMENT",
          "UNARY_OPERATORS_LOGICAL_NOT",
          "UNARY_OPERATORS_NEGATIVE",
          "UNARY_OPERATORS_POSITIVE",
          "INTEGER_NUMERIC_LITERALS",
          "FLOATING_NUMERIC_LITERALS",
          "IDENTIFIERS",
          NULL,
      };

  for (int i = 0; filename[i] != NULL; i++)
  {
    insert_new_automaton_in_node_automaton(automaton_rules_list, automaton_rules_list->final_automaton, filename[i]);
  }
  printf("Carregados [ %d ] automatos\n", automaton_rules_list->files_reads);
  ensure_zeroA_end_file(path_analyser_file);
  gcolored_print("[SUCCESS] ->", GREEN);
  printf("Automatos carregados.\n\n");
  printf("\n________________________________________________\n");
  control_read_file_in_c_and_make_token_fill_symbol_table_and_token_list(automaton_rules_list, tokens_of_input_files, symbol_of_table, path_analyser_file);
  printf("\n________________________________________________\n");
  gcolored_print("\n\n[SUCCESS] ->", GREEN);
  printf("tokens gerados.\n");
  return 0;
}
