//  CITS2002 Project 1 2022
//  Student1:   23062249   LIN ZHIHAO
//  Student2:   23215129   DUNLOP LIAM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int is_now(char *file_time, int time) {
  if (*file_time == '*') {
    return 1;
  }
  char time_str[3];
  sprintf(time_str, "%i", time);
  
  if (strcmp(file_time, time_str) == 0) {
    return 1;
  }
  return 0;
}

int return_month(char *month_str) {
  char months[12][4] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
  char int_months[12][3] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"};

  int month_index = -1;
  for (int i = 0; i<12; i++) {
    if (strcmp(months[i], month_str) == 0 || strcmp(int_months[i], month_str) == 0) {
      month_index = i;
    }
  }
  return month_index;  
}

int check_month(char *file_time, char *month) {
  if (*file_time == '*') {
    return 1;
  // If both month in file and month in input are numeric or both three letter abbr., will be returned here.
  } else if (strcmp(month, file_time) == 0) {
    return 1;
  } 
  
  int file_month = return_month(file_time);
  int input_month = return_month(month);

  if (file_month != -1 && input_month != -1) {
    if (file_month == input_month) {
      return 1;
    }
  } else {
    return 0;
  }
  return 0;
}

int check_weekday(char *file_time, char *month, int day) {
  char weekdays[7][4] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
  char weekday_int[7][2] = {"0", "1", "2", "3", "4", "5", "6"};
  if (*file_time == '*') {
    return 1;
  }

  int weekday = -1;
  for (int i = 0; i<7; i++) {
    if (strcmp(file_time, weekdays[i]) == 0 || strcmp(file_time, weekday_int[i]) == 0) {
      weekday = i;
    }
  }

  struct tm time;
  memset(&time, 0, sizeof time);
  time.tm_mday = day;
  time.tm_mon = return_month(month);
  time.tm_year = 2022 - 1900;

  mktime(&time);
  if (weekday == time.tm_wday) {
    return 1;
  }
  return 0;
}

void simulate(char *tasks[], char *task_estimates[], char *month, int* total_command, int* max_command, char* most_command){
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
      } else if (is_now(tasks[i*6+MINUTE_INDEX], minutes) == 1) {
        // printf("%s is equal to %i\n", tasks[i*6+MINUTE_INDEX], minutes);
        if (is_now(tasks[i*6+HOUR_INDEX], hours) == 1) {
          if (is_now(tasks[i*6+DAY_INDEX], day) == 1) {
            if (check_month(tasks[i*6+MONTH_INDEX], month) == 1) {
              if (check_weekday(tasks[i*6+WEEKDAY_INDEX], month, day) == 1) {
                printf("The day is %i, and the task is %s\n", day, tasks[i*6+TASK_NAME_INDEX]);
              }
              
            }
          } 
        } else {
          // printf("File is %s, hour is %i", tasks[i*6+HOUR_INDEX], hours);
        }
        // printf("The day is %i, and the task is %s\n", day, tasks[i*6+TASK_NAME_INDEX]);
      }
    } 
    minutes = minutes + 1;
  }
}

void parse_file(FILE *ptr, char *buf[], char *dest[], int columns) {
  int i = 0;
  char line[100];
  while (fgets(line, sizeof line, ptr) != NULL) {
    if (line[0] == '#') {
      continue;
    }
    get_date(line, buf);
    for (int j=0; j<columns; j++) {
      dest[i*columns+j] = (char *) malloc(sizeof(buf[j])+1);
      strcpy(dest[i*columns+j], buf[j]);
    }
    i = i + 1;
  }
}


int main(int argc, char *argv[]) {
  FILE *schedule, *estimates;
  // char line[100];
  char *date[6];
  char *tasks[120] = {NULL};
  char *task_times[20] = {NULL};

  schedule = fopen("crontab-file.txt", "r");
  estimates = fopen("estimates-file.txt", "r");
  if (schedule==NULL || estimates == NULL) {
    printf("error opening file\n");
  }

  // Reading file into array
  parse_file(schedule, date, tasks, 6);
  parse_file(estimates, date, task_times, 2);
  // int j = 0;
  // while (fgets(line, sizeof line, schedule) != NULL) {
  //   if (line[0] == '#') {
  //     continue;
  //   }
  //   get_date(line, date);
  //   for (int i=0; i<6; i++){
  //     tasks[j*6+i] = (char *) malloc(sizeof(date[i])+1);
  //     strcpy(tasks[j*6+i], date[i]);
  //   }
  //   j = j + 1;
  // }

  // int m = 0;
  // while (fgets(line, sizeof line, estimates) != NULL) {
  //   if (line[0] == '#') {
  //     continue;
  //   }
  //   get_date(line, date);
  //   for (int n=0; n<2; n++) {
  //     task_times[m*2+n] = (char *) malloc(sizeof(date[n])+1);
  //     strcpy(task_times[m*2+n], date[n]);
  //   }
  //   m = m + 1;
  // }

  int *total_command = 0;
  int *max_command = 0;
  char *most_command = (char *) malloc(40);
  simulate(tasks, task_times, argv[1], total_command, max_command, most_command);
  // for (int k=0; k<5; k++) {
  //   for (int l=0; l<2; l++){
  //     if (task_times[k*2+l] == NULL) {
  //       break;
  //     }
  //     printf("%s\n", task_times[k*2+l]);
  //   }
  // }

  fclose(schedule);
  fclose(estimates);
  return 0;
}


