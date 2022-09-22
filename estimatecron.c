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

void get_date(char line[], char *buf[]) {
  // Function to split line by spaces and place into buffer array.
  char *newline = strchr(line, '\n');
  if (newline){
    *newline = 0;
  }
  char* ptr = strtok(line, " ");
  int i = 0;
  while (ptr != NULL) {
    buf[i] = ptr;
    ptr = strtok(NULL, " ");
    i = i + 1;
  }
}

int is_now(char *file_time, int time) {
  // Function to check if time stored in variable is the same as time in file.
  if (*file_time == '*') {
    return 1;
  }
  char time_str[3];
  sprintf(time_str, "%i", time);
  // Convert int to string
  if (strcmp(file_time, time_str) == 0) {
    return 1;
  }
  return 0;
}

int return_month(char *month_str) {
  // Return month as an integer, either from a three letter word, or a string integer
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
  
  // Else if one is a string, and one is an integer as a string, will both be converted to integers here and compared.

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

  // Convert weekday from either three letter word or integer string to integer

  int weekday = -1;
  for (int i = 0; i<7; i++) {
    if (strcmp(file_time, weekdays[i]) == 0 || strcmp(file_time, weekday_int[i]) == 0) {
      weekday = i;
    }
  }

  // Check date in year is actually the specified weekday.
  
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

int process_index(char *process, char *process_estimates[]) {
  // Return the index of the process name in estimates file
  for (int i = 0; i < 20; i++) {
    if (process_estimates[i*2] == NULL) {
      break;
    }
    if (strcmp(process, process_estimates[i*2]) == 0) {
      return i;
    }
  }
}

int simulate(char *tasks[], char *task_estimates[], char *month, int* total_command, int* max_command, char* most_command){
  int minutes = 0;
  int hours = 0;
  int day = 1;
  int days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int current_month = return_month(month);

  int processes_run = 0;
  int n_running = 0;
  int most_running = 0;

  int process_count_down[20] = {0};
  int process_counter[20] = {0};

  while (day < days_in_month[current_month] + 1) {
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
        if (is_now(tasks[i*6+HOUR_INDEX], hours) == 1) {
          if (is_now(tasks[i*6+DAY_INDEX], day) == 1) {
            if (check_month(tasks[i*6+MONTH_INDEX], month) == 1) {
              if (check_weekday(tasks[i*6+WEEKDAY_INDEX], month, day) == 1) {
                if (n_running < 20) {
                  n_running = n_running + 1;
                  processes_run = processes_run + 1;
                  int task_index = process_index(tasks[i*6+TASK_NAME_INDEX], task_estimates);
                  // printf("%s, %i\n", tasks[i*6+TASK_NAME_INDEX], task_index);
                  process_counter[task_index]++;
                  for (int j = 0; j < 20; j++) {
                    if (process_count_down[j] == 0) {
                      process_count_down[j] = atoi(task_estimates[task_index*2+1]);
                      break;
                    }
                  }
                }
              }
            }
          } 
        }
      }
    } 
    for (int k = 0; k < 20; k++) {
      if (process_count_down[k] == 0){
        continue;
      } else if (process_count_down[k] == 1) {
        n_running--;
      }
      process_count_down[k]--;
    }

    if (n_running > most_running) {
      most_running = n_running;
    }
    // If nrunning is larger than max_command, change value of max command
    minutes = minutes + 1;
  }
  *total_command = processes_run;
  *max_command = most_running;

  int max = 0;
  int max_index = -1;
  for (int l = 0; l < 20; l++) {
    if (process_counter[l] > max) {
      max = process_counter[l];
      max_index = l;
    }
  }
  
  return max_index;
}

void parse_file(FILE *ptr, char *buf[], char *dest[], int columns) {

  // Read file into array

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
  char *date[6];
  char *tasks[120] = {NULL};
  char *task_times[20] = {NULL};

  char f1[256];
  strcpy(f1, argv[2]);
  strcat(f1, ".txt");

  char f2[256];
  strcpy(f2, argv[3]);
  strcat(f2, ".txt");

  schedule = fopen(f1, "r");
  estimates = fopen(f2, "r");
  if (schedule == NULL || estimates == NULL) {
    printf("error opening file\n");
    exit(0);
  }

  // Reading file into array
  parse_file(schedule, date, tasks, 6);
  parse_file(estimates, date, task_times, 2);
  
  int *total_command = malloc(sizeof total_command);
  int *max_command = malloc(sizeof max_command);
  char *most_command = (char *) malloc(41);
  int max_index = simulate(tasks, task_times, argv[1], total_command, max_command, most_command);
  
  if (max_index == -1) {
    char def[5] = "none";
    printf("%s\t%i\t%i\n", def, *total_command, *max_command);
  } else {
    most_command = task_times[max_index*2];
    printf("%s\t%i\t%i\n", most_command, *total_command, *max_command);
  }

  free(total_command);
  free(max_command);
  free(most_command);
  fclose(schedule);
  fclose(estimates);
  return 0;
}


