#include <stdio.h>
#include <stdlib.h>
#include "../includes/gstr.h"

int gstrlen(char *str)
{
  int size = 0;
  if (str[1] == '\0')
  {
    return 1;
  }
  while (str[size] != '\0')
  {
    size++;
  }
  return size;
}

char *gstrcopy(char *str)
{
  int size = gstrlen(str) + 1, i = 0;
  char *gstr = (char *)calloc(size, sizeof(char));
  while (i < (size - 1))
  {
    gstr[i] = str[i];
    i++;
  }
  return gstr;
}

char *gstraloc(char *str)
{
  if (str == NULL)
  {
    return NULL;
  }

  char *new = (char *)malloc((gstrlen(str) + 1) * sizeof(char));
  new = gstrcopy(str);
  return new;
}

void gstrsetvalue(char **str, int position, char value)
{
  (*str)[position] = value;
}

void gmemset(char **str, int size, char set)
{
  int i = 0;
  while (i < size)
  {
    *((*str) + i) = set;
    // *str[i] = set;
    i++;
  }
}

bool gstrcomp(char *gstr, char *str)
{
  if ((gstrlen(gstr) == 1) && (gstrlen(str) == 1))
  {
    if (*gstr == *str)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  if (gstr == NULL || str == NULL)
  {
    return false;
  }

  if (gstrlen(gstr) != gstrlen(str))
  {
    return false;
  }

  int i = 0;
  while (gstr[i] != '\0')
  {
    if (gstr[i] != str[i])
    {
      return false;
    }
    i++;
  }
  return true;
}

bool is_empty(char *str)
{
  if (str[0] == '\0')
  {
    return true;
  }
  return false;
}

bool gcharcomp(char gstr, char str)
{
  if (gstr == str)
  {
    return true;
  }
  return false;
}

int gstr_to_int(char *gstr, int size)
{
  int i = 0, result = 0;
  while (i <= size)
  {
    result = result * 10;
    result = result + (gstr[i] - 48);
    i++;
  }
  return result;
}

void ensure_zeroA_end_file(char *filename)
{
  FILE *file = fopen(filename, "r");
  if ((file == NULL))
  {
    printf("\n\n\033[41;1;37m[ERROR]\033[m\tOpening file for verification\n");
    exit(EXIT_FAILURE);
  }
  char buffer;
  fseek(file, -1, SEEK_END);
  fread(&buffer, sizeof(char), 1, file);
  fclose(file);
  if (buffer == '\n')
  {
    return;
  }
  else
  {
    file = fopen(filename, "a");
    if ((file == NULL))
    {
      printf("\n\n\033[41;1;37m[ERROR]\033[m\tOpening file for verification\n");
      exit(EXIT_FAILURE);
    }
    fprintf(file, "\n");
    fclose(file);
  }
}

void gcolored_print(char *text, char *color)
{
  printf("%s%s%s", color, text, RESET);
}