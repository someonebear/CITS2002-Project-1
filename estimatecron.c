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
  char *date[6];
  char *tasks[120];

  dict = fopen("crontab-file.txt", "r");
  if (dict==NULL) {
    printf("error opening file\n");
  }

  int j = 0;
  while(fgets(line, sizeof line, dict) != NULL) {
    if (line[0] == '#') {
      continue;
    }
    get_date(line, date);
    for (int i=0; i<6; i++){
      tasks[j*6+i] = (char *) malloc(sizeof(date[i])+1);
      strcpy(tasks[j*6+i], date[i]);
    }
    j = j + 1;
  }

  // for (int k=0; k<5; k++) {
  //   for (int l=0; l<6; l++){
  //     printf("%s\n", tasks[k*6+l]);
  //   }
  // }

  fclose(dict);
  free(tasks);
  return 0;
}