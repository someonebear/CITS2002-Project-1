#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MONTH_INDEX 3
#define DAY_INDEX 2

void get_date(char line[], char *buf[]) {
  char* ptr = strtok(line, " ");
  int i = 0;
  while (ptr != NULL) {
    buf[i] = ptr;
    ptr = strtok(NULL, " ");
    i = i + 1;
  }
}

int main(int argc, char *argv[]) {
  FILE *dict;
  char line[100];
  char *date[5];

  dict = fopen("crontab-file.txt", "r");
  if (dict==NULL) {
    printf("error opening file\n");
  }

  while(fgets(line, sizeof line, dict) != NULL) {
    if (line[0] == '#') {
      continue;
    }
    get_date(line, date);
    for (int i=0; i<5; i++){
      printf("%s\n", date[i]);
    }
  }

  fclose(dict);
  return 0;
}