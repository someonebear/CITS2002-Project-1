#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MONTH_INDEX 3

char get_date(char line[], int date_type) {
  char* ptr = strtok(line, " ");
  int i = 0;
  while (ptr != NULL && i <= date_type) {
    if (i == date_type) {
      return *ptr;
    }
    ptr = strtok(NULL, " ");
    i = i + 1;
  }
  return '*';
}

int main(int argc, char *argv[]) {
  FILE *dict;
  char line[100];
  char month;

  dict = fopen("crontab-file.txt", "r");
  if (dict==NULL) {
    printf("error opening file\n");
  }

  while(fgets(line, sizeof line, dict) != NULL) {
    if (line[0] == '#') {
      continue;
    }
    month = get_date(line, MONTH_INDEX);
    if (month == *argv[1]) {
      printf("This is the %cth month of the year\n", month);
    }
  }

  fclose(dict);
  return 0;
}