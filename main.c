#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define CITY_LEN 672
#define MAX_LINE_LEN 106
#define FILENAME "20220211_isolamento.csv"

typedef struct {
  char *city;
  int code;
  int population;
  char *state;
  char *date;
  int isolation;
} row;

void println_info(row *x);
int menu();
void line();
void line2();

FILE *open_read_file(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error! Couldn't open file '%s' - %s\n", filename,
            strerror(errno));
    exit(1);
  }
  return file;
}

FILE *open_write_file() {
  const char *output = "out.csv";
  struct stat buf;

  FILE *out = fopen(output, "a");
  if (!out) {
    fprintf(stderr, "Error! Couldn't open file '%s' - %s\n", output,
            strerror(errno));
    exit(1);
  }

  if (stat(output, &buf) == 0) {
    FILE *rfile = open_read_file(FILENAME);
    char *line = malloc(sizeof(char) * MAX_LINE_LEN);
    fgets(line, MAX_LINE_LEN, rfile);
    fprintf(out, "%s", line);
    fclose(rfile);
  }

  return out;
}

void tokenize(char *line, row *copy) {
  char *token = NULL;
  token = strtok(line, ";");
  copy->city = malloc(strlen(token) + 1);
  strcpy(copy->city, token);

  token = strtok(NULL, ";");
  copy->code = atoi(token);

  token = strtok(NULL, ";");
  copy->population = atoi(token);

  token = strtok(NULL, ";");
  copy->state = malloc(strlen(token) + 1);
  strcpy(copy->state, token);

  token = strtok(NULL, ";");
  copy->date = malloc(strlen(token) + 1);
  strcpy(copy->date, token);

  token = strtok(NULL, "%%");
  copy->isolation = atoi(token);
}

void get_city(int usr_code, char *cities[]) {
  FILE *file = open_read_file(FILENAME);

  char *line = malloc(sizeof(char) * MAX_LINE_LEN);
  row *copy = malloc(sizeof(row));
  char *token = NULL;

  int cnt = 0;

  while (fgets(line, MAX_LINE_LEN, file)) {

    char *line_copy = malloc(sizeof(char) * MAX_LINE_LEN);
    line_copy = strcpy(line_copy, line);

    token = strtok(line, ";");
    token = strtok(NULL, ";");
    copy->code = *(int *)malloc(sizeof(token));
    copy->code = atoi(token);

    if (usr_code == copy->code) {
      cities[cnt] = line_copy;
      cnt++;
    }

    free(copy);
    free(line);
    copy = malloc(sizeof(row));
    line = malloc(sizeof(char) * MAX_LINE_LEN);
  }
  free(line);
  fclose(file);

  printf("Found: %d\n", cnt);
  if (cnt == 0)
    return;

  char save_op;
  printf("Do you wish to write to the output file? [Y/n] ");
  scanf(" %c", &save_op);

  if (save_op == 'y' || save_op == 'Y') {
    FILE *wfile = open_write_file();
    for (int i = 0; i < CITY_LEN; i++) {
      fprintf(wfile, "%s", cities[i]);
    }
    printf("File saved successfully!\n");
    fclose(wfile);
  }
}

double get_average() {
  FILE *file = open_read_file(FILENAME);

  float sum = 0;
  int line_idx = 0;

  // Skip header
  char *header = malloc(sizeof(char) * MAX_LINE_LEN);
  fgets(header, MAX_LINE_LEN, file);
  free(header);

  char *line = malloc(sizeof(char) * MAX_LINE_LEN);

  while (fgets(line, MAX_LINE_LEN, file)) {

    row *copy = malloc(sizeof(row));

    tokenize(line, copy);

    println_info(copy);

    sum += copy->isolation;
    line_idx++;

    free(copy->date);
    free(copy->state);
    free(copy->city);
    free(copy);
    free(line);
    line = malloc(sizeof(char) * MAX_LINE_LEN);
  }
  free(line);
  fclose(file);

  return sum / line_idx;
}

void get_average_by_city(char *cities[]) {
  if (!cities[0]) {
    printf("\x1b[33m"
           "\nNo city in memory (Try option [1] first)\n"
           "\x1b[0m");
    return;
  }

  float sum = 0;
  char *city_name = NULL;

  for (int i = 0; i < CITY_LEN; i++) {
    row *copy = malloc(sizeof(row));
    tokenize(cities[i], copy);
    city_name = copy->city;
    sum += copy->isolation;
    free(copy);
    copy = malloc(sizeof(row));
  }
  printf("\nAverage in %s: %.3f%%\n", city_name, sum / CITY_LEN);
}

int main() {
  char *cities[CITY_LEN];

  for (;;) {
    int option = menu();
    int city_code;

    switch (option) {
    case 1:
      printf("Enter city code: ");
      scanf("%d", &city_code);
      get_city(city_code, cities);
      break;
    case 2:
      get_average_by_city(cities);
      break;
    case 3:
      printf("\nTotal Average: %.3f%%\n", get_average());
      break;
    case 4:
      printf("Bye!\n");
      return 0;
    default:
      printf("\x1b[31m"
             "\nChoose a valid option!\n"
             "\x1b[0m");
      break;
    }
  }
}

void println_info(row *x) {
  printf("\nCity: %s\n", x->city);
  printf("Code: %d\n", x->code);
  printf("Population: %d\n", x->population);
  printf("State: %s\n", x->state);
  printf("Date: %s\n", x->date);
  printf("Isolation: %d%%\n", x->isolation);
}

int menu() {
  int op;
  printf("\n");
  line();
  line2();
  printf("*  Isolation data research  *\n");
  line2();
  line();
  printf("\n[1] Search by city\n");
  printf("[2] Get city average\n");
  printf("[3] Get total average\n");
  printf("[4] Exit\n");
  printf("\nChoose an option: ");
  if (scanf("%d", &op) != 1) {
    while (getchar() != '\n') {
    }
    return -1;
  }
  return op;
}

void line() {
  int i;
  for (i = 0; i <= 13; i++) {
    printf("*.");
  }
  printf("*\n");
}

void line2() {
  int i;
  printf("|");
  for (i = 0; i <= 26; i++) {
    printf(" ");
  }
  printf("|\n");
}
