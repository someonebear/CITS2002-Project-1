#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TASK_NAME_INDEX 5
#define WEEKDAY_INDEX 4
#define MONTH_INDEX 3
#define DAY_INDEX 2
#define HOUR_INDEX 1
#define MINUTE_INDEX 0

struct Process {
  char pname[50];
  int pid;
  int runtime;
  int running;
};

void get_date(char line[], char *buf[]) {
  char* ptr = strtok(line, " ");
  int i = 0;
  while (ptr != NULL) {
    buf[i] = ptr;
    ptr = strtok(NULL, " ");
    i = i + 1;
  }
}

void simulate(char *tasks[], int month, int* total_command, int* max_command, char* most_command){
  int minutes = 0;
  int hours = 0;
  int day = 1;

  while (day<31) {
    if (minutes == 60) {
      hours = hours + 1;
      minutes = 0;
    }
    if (hours == 24) {
      day = day + 1;
      hours = 0;
    }

    for (int i = 0; i < 20; i++) {
      if (tasks[i*6+TASK_NAME_INDEX] == NULL) {
        break;
      } else if (*tasks[i*6+DAY_INDEX] == day || *tasks[i*6+DAY_INDEX] == '*') {
        // printf("The day is %i, and the task is %s\n", day, tasks[i*6+TASK_NAME_INDEX]);
      }
    } 
    // printf("Minute:%i, Hour:%i, Day:%i", minutes, hours, day);
    minutes = minutes + 1;
  }
}



int main(int argc, char *argv[]) {
  FILE *schedule, *estimates;
  char line[100];
  char *date[6];
  char *tasks[120] = {NULL};
  char *task_times[20] = {NULL};

  schedule = fopen("crontab-file.txt", "r");
  estimates = fopen("estimates-file.txt", "r");
  if (schedule==NULL || estimates == NULL) {
    printf("error opening file\n");
  }

  // Reading file into array
  int j = 0;
  while (fgets(line, sizeof line, schedule) != NULL) {
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

  int m = 0;
  while (fgets(line, sizeof line, estimates) != NULL) {
    if (line[0] == '#') {
      continue;
    }
    get_date(line, date);
    for (int n=0; n<2; n++) {
      task_times[m*2+n] = (char *) malloc(sizeof(date[n])+1);
      strcpy(task_times[m*2+n], date[n]);
    }
    m = m + 1;
  }

  int *total_command = 0;
  int *max_command = 0;
  char *most_command = (char *) malloc(20);
  simulate(tasks, *argv[1], total_command, max_command, most_command);
  // for (int k=0; k<5; k++) {
  //   for (int l=0; l<2; l++){
  //     if (task_times[k*2+l] == NULL) {
  //       break;
  //     }
  //     printf("%s\n", task_times[k*2+l]);
  //   }
  // }

  fclose(schedule);
  return 0;
}


