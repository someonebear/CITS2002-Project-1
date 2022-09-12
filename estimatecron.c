#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MONTH_INDEX 3

char get_date(char line[], int date_type) {
  char *ptr = strtok(line, " ");
  int i = 0;
  while (ptr != NULL && i <= date_type) {
    if (i == date_type) {
      printf("%s\n", ptr);
      return *ptr;
    }
    ptr = strtok(NULL, " ");
    i = i + 1;
  }
  return '*';
}

int main() {
  FILE *dict;
  char line[100];
  dict = fopen("crontab-file.txt", "r");

  if (dict==NULL) {
    printf("error opening file\n");
  }

  while(fgets(line, sizeof line, dict) != NULL) {
    if (line[0] == '#') {
      continue;
    }
    get_date(line, MONTH_INDEX);
    // printf("%s", line);
  }

  fclose(dict);
  return 0;
}