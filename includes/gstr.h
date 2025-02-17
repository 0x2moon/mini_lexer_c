#ifndef GSTR_H
#define GSTR_H
#include <stdbool.h>
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
#endif
