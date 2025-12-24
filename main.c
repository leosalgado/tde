#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define WIDTH 27
#define MAX_LINE_LEN 106
#define FILENAME "20220211_isolamento.csv"
#define OUTPUT "out.csv"

typedef struct {
  char *city;
  char *state;
  char *date;
  int code;
  int population;
  int isolation;
} Row;

void menu_prompt();

void println_info(Row *r) {
  printf("\nCity: %s\n", r->city);
  printf("Code: %d\n", r->code);
  printf("Population: %d\n", r->population);
  printf("State: %s\n", r->state);
  printf("Date: %s\n", r->date);
  printf("Isolation: %d%%\n", r->isolation);
}

FILE *xfopen(const char *filename, const char *mode) {
  FILE *file = fopen(filename, mode);
  if (!file) {
    fprintf(stderr, "Error! Couldn't open file '%s' - %s\n", filename,
            strerror(errno));
    exit(1);
  }

  return file;
}

void skip_header(FILE *file) {
  char *header = malloc(sizeof(char) * MAX_LINE_LEN);
  fgets(header, MAX_LINE_LEN, file);
  free(header);
}

int realloc_and_copy_string(char **dest, const char *src) {
  size_t len = strlen(src) + 1;
  char *tmp = realloc(*dest, len);
  if (tmp == NULL)
    return 1;

  *dest = tmp;
  memcpy(*dest, src, len);
  return 0;
}

void tokenize(char *line, Row *row) {
  char *token;

  token = strtok(line, ";");
  if (realloc_and_copy_string(&row->city, token))
    return;

  token = strtok(NULL, ";");
  row->code = atoi(token);

  token = strtok(NULL, ";");
  row->population = atoi(token);

  token = strtok(NULL, ";");
  if (realloc_and_copy_string(&row->state, token))
    return;

  token = strtok(NULL, ";");
  if (realloc_and_copy_string(&row->date, token))
    return;

  token = strtok(NULL, "%");
  row->isolation = atoi(token);
}

void row_init(Row *row) {
  row->city = NULL;
  row->state = NULL;
  row->date = NULL;
}

void row_clear(Row *row) {
  if (!row)
    return;

  free(row->city);
  free(row->state);
  free(row->date);

  row_init(row);
}

void row_destroy(Row *row) {
  if (!row)
    return;

  row_clear(row);
  free(row);
}

void get_city(int usr_code, FILE *file) {
  Row *row = malloc(sizeof(Row));
  char *line = malloc(sizeof(char) * MAX_LINE_LEN);
  char *line_copy = malloc(sizeof(char) * MAX_LINE_LEN);
  if (!row || !line || !line_copy)
    goto cleanup;

  row_init(row);

  FILE *tmp = tmpfile();
  if (!tmp)
    goto cleanup;

  int cnt = 0;

  while (fgets(line, MAX_LINE_LEN, file)) {
    strcpy(line_copy, line);
    tokenize(line, row);

    if (usr_code == row->code) {
      fprintf(tmp, "%s", line_copy);
      cnt++;
    }
  }

  printf("Found: %d\n", cnt);
  if (cnt < 1)
    goto cleanup;

  char save_op;
  printf("Do you wish to save city in memory? [Y/n] ");
  scanf(" %c", &save_op);

  if (save_op == 'y' || save_op == 'Y') {
    rewind(tmp);
    FILE *wfile = xfopen(OUTPUT, "w");

    while (fgets(line, MAX_LINE_LEN, tmp))
      fprintf(wfile, "%s", line);

    puts("\x1b[32m"
         "\nCity saved successfully!"
         "\x1b[0m");
    fclose(wfile);
  }

cleanup:
  if (tmp)
    fclose(tmp);
  if (row)
    row_destroy(row);
  free(line);
  free(line_copy);
}

double get_average(FILE *file) {
  float sum = 0.0F;
  int line_idx = 0;

  Row *row = malloc(sizeof(Row));
  char *line = malloc(sizeof(char) * MAX_LINE_LEN);
  if (!row || !line)
    goto cleanup;

  row_init(row);

  while (fgets(line, MAX_LINE_LEN, file)) {
    tokenize(line, row);
    println_info(row);

    sum += row->isolation;
    line_idx++;
  }

cleanup:
  if (row)
    row_destroy(row);
  free(line);

  if (line_idx == 0)
    return 0.0;

  return sum / line_idx;
}

void handle_option(const char option) {
  switch (option) {
  case '1': {
    int city_code;
    printf("Enter city code: ");
    scanf("%d", &city_code);
    FILE *file = xfopen(FILENAME, "r");
    get_city(city_code, file);
    fclose(file);
    break;
  }
  case '2': {
    FILE *file = fopen(OUTPUT, "r");
    if (!file) {
      puts("\x1b[33m"
           "\nNo city in memory, try option [1] first!"
           "\x1b[0m");
      break;
    }
    printf("\nCity Average: %.3f%%\n", get_average(file));
    fclose(file);
    break;
  }
  case '3': {
    FILE *file = xfopen(FILENAME, "r");
    skip_header(file);
    printf("\nTotal Average: %.3f%%\n", get_average(file));
    fclose(file);
    break;
  }
  default:
    puts("\x1b[31m"
         "\n\nChoose a valid option!"
         "\x1b[0m");
    break;
  }
}

char get_option() {
  char op;
  if (scanf(" %c", &op) != 1) {
    while (getchar() != '\n')
      ;
    return -1;
  }

  return op;
}

void menu_loop() {
  for (;;) {
    menu_prompt();
    char option = get_option();
    if (option == 'q')
      break;
    handle_option(option);
  }
}

int main() {
  menu_loop();
  remove(OUTPUT);
  puts("Bye!");
  return 0;
}

void print_line(char c) {
  for (int i = 0; i < WIDTH; i++)
    putchar(c);
}

void hline() {
  putchar('+');
  print_line('-');
  puts("+");
}

void vline() {
  putchar('|');
  print_line(' ');
  puts("|");
}

void menu_prompt() {
  puts("");
  hline();
  vline();
  puts("|  Isolation data research  |");
  vline();
  hline();

  puts("");
  puts("[1] Search by city");
  puts("[2] Get city average");
  puts("[3] Get total average");
  puts("");
  puts("(Use 'q' to exit)");
  puts("");
  printf("Choose an option: ");
}
