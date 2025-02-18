#ifndef GSTR_H
#define GSTR_H
#include <stdbool.h>

#define RED "\033[41;1;37m"
#define GREEN "\033[42;1;30m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[30;47m"
#define RESET "\033[0m"

char *gstraloc(char *str);
int gstrlen(char *str);
void gstrsetvalue(char **str, int position, char value);
void gmemset(char **str,int size, char set);
char *gstrcopy(char *str);
bool gstrcomp(char *gstr, char *str);
bool gcharcomp(char gstr, char str);
bool is_empty(char *str);
int gstr_to_int(char *gstr, int size);
void ensure_zeroA_end_file(char *filename);
void gcolored_print(char *text, char *color);
#endif
