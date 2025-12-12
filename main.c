#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define V_SIZE 24002
// original file: 93758 lines -> 25.6%

int query1(int, int, int, int, char[*][*], int[*], int[*], char[*][*],
           char[*][*], int[*]);
float query2(int, int, int, int, char[*][*], int[*], int[*], char[*][*],
             char[*][*], int[*]);
float query3(int, int, int[*], int[*], int[*]);
int save();
int menu(void);
int menu2(void);
void line();
void line2();
void city_name(int, int, int, char[*][*], int[*]);

int main() {

  FILE *file;
  FILE *out;

  int option = 1;
  char city[V_SIZE][25];
  int code[V_SIZE];
  int population[V_SIZE];
  char state[V_SIZE][2];
  char date[V_SIZE][30];
  int isolation[V_SIZE];

  file = fopen("20220211_isolamento.csv", "r");

  if (!file) {
    printf("Error! Couldn't open file to read");
    return 1;
  }

  char line[150];
  fgets(line, 150, file);

  char *token;
  int n_lines = 0;
  while (n_lines < V_SIZE) {
    fgets(line, 150, file);
    token = strtok(line, ";");
    strcpy(city[n_lines], token);
    token = strtok(NULL, ";");
    code[n_lines] = atoi(token);
    token = strtok(NULL, ";");
    population[n_lines] = atoi(token);
    token = strtok(NULL, ";");
    strcpy(state[n_lines], token);
    token = strtok(NULL, ";");
    strcpy(date[n_lines], token);
    token = strtok(NULL, "%%");
    isolation[n_lines] = atoi(token);
    n_lines++;
  }

  fclose(file);

  out = fopen("out.csv", "w");

  if (!out) {
    printf("Error while opening output file!\n");
    return 1;
  }

  int extra = 0, aux_code, option_save, i;
  char code_STRING[V_SIZE][7];
  char pop_STRING[V_SIZE][8];
  char isol_STRING[V_SIZE][2];

  while (option != 3) {
    printf("\n");
    if (extra == 0) {
      option = menu();
      switch (option) {
      case 1:
        aux_code = query1(V_SIZE, 25, 2, 30, city, code, population, state,
                          date, isolation);
        option_save = save();
        extra++;
        if (option_save == 1) {
          printf("\nFile saved successfully!");
          fputs("Município1;Código Município IBGE;População estimada "
                "(2020);UF1;Data;Média de Índice De Isolamento\n",
                out);
          for (i = 0; i < V_SIZE; i++) {
            if (aux_code == code[i]) {
              strcpy(line, city[i]);
              strcat(line, ";");
              sprintf(code_STRING[i], "%d", code[i]);
              strcat(line, code_STRING[i]);
              strcat(line, ";");
              sprintf(pop_STRING[i], "%d", population[i]);
              strcat(line, pop_STRING[i]);
              strcat(line, ";");
              strcat(line, "SP");
              strcat(line, ";");
              strcat(line, date[i]);
              strcat(line, ";");
              sprintf(isol_STRING[i], "%d", isolation[i]);
              strcat(line, isol_STRING[i]);
              strcat(line, "%");
              fprintf(out, "%s\n", line);
            }
          }
        }
        break;
      case 2:
        printf("\nAverage = %.2f", query2(V_SIZE, 25, 2, 30, city, code,
                                          population, state, date, isolation));
        break;
      case 3:
        printf("\nBye!");
        break;
      default:
        printf("Invalid option!");
      }
    } else {
      option = menu2();
      switch (option) {
      case 1:
        aux_code = query1(V_SIZE, 25, 2, 30, city, code, population, state,
                          date, isolation);
        option_save = save();
        if (option_save == 1) {
          printf("\nFile saved successfully!");
          fputs("Município1;Código Município IBGE;População estimada "
                "(2020);UF1;Data;Média de Índice De Isolamento\n",
                out);
          for (i = 0; i < V_SIZE; i++) {
            if (aux_code == code[i]) {
              strcpy(line, city[i]);
              strcat(line, ";");
              sprintf(code_STRING[i], "%d", code[i]);
              strcat(line, code_STRING[i]);
              strcat(line, ";");
              sprintf(pop_STRING[i], "%d", population[i]);
              strcat(line, pop_STRING[i]);
              strcat(line, ";");
              strcat(line, "SP");
              strcat(line, ";");
              strcat(line, date[i]);
              strcat(line, ";");
              sprintf(isol_STRING[i], "%d", isolation[i]);
              strcat(line, isol_STRING[i]);
              strcat(line, "%");
              fprintf(out, "%s\n", line);
            }
          }
        }
        break;
      case 2:
        printf("\nAverage = %.2f", query2(V_SIZE, 25, 2, 30, city, code,
                                          population, state, date, isolation));
        break;
      case 3:
        printf("\nBye!");
        break;
      case 4:
        city_name(aux_code, V_SIZE, 25, city, code);
        printf("\nCity's average = %.2f",
               query3(aux_code, V_SIZE, code, population, isolation));
        break;
      default:
        printf("Invalid option!");
      }
    }
    printf("\n");
  }
  fclose(file);

  return 0;
}

void city_name(int cod, int lim, int lim2, char v[lim][lim2], int j[lim]) {
  int i;
  for (i = 0; i < lim; i++) {
    if (cod == j[i]) {
      printf("\n*%s*", v[i]);
      break;
    }
  }
}

int save() {
  int option;
  printf("Do you wish to save the file?\n\n");
  printf("[1] YES\n");
  printf("[2] NO\n\n");
  printf("Choose an option: ");
  scanf("%d", &option);
  return option;
}

int query1(int lim, int lim2, int lim3, int lim4, char v[lim][lim2], int j[lim],
           int k[lim], char l[lim][lim3], char m[lim][lim4], int n[lim]) {
  int i;
  int cod;

  printf("City's code: ");
  scanf("%i", &cod);

  for (i = 0; i < lim; i++) {
    if (cod == j[i]) {
      printf("%s ", v[i]);
      printf("%i ", j[i]);
      printf("%i ", k[i]);
      printf("%.2s ", l[i]);
      printf("%s ", m[i]);
      printf("%i%% ", n[i]);
      printf("\n\n");
    }
  }
  return cod;
}

float query2(int lim, int lim2, int lim3, int lim4, char v[lim][lim2],
             int j[lim], int k[lim], char l[lim][lim3], char m[lim][lim4],
             int n[lim]) {

  float average = 0;
  int i;

  for (i = 0; i < lim; i++) {
    average = average + (k[i] + n[i]);
  }

  average = average / lim;

  return average;
}

float query3(int cod, int lim, int j[lim], int k[lim], int n[lim]) {

  float average = 0;
  int i, x = 0;

  for (i = 0; i < lim; i++) {
    if (cod == j[i]) {
      average = average + (k[i] + n[i]);
      x++;
    }
  }

  average = average / x;

  return average;
}

int menu() {
  int op;
  line();
  line2();
  printf("*  Isolation data research  *\n");
  line2();
  line();
  printf("\n[1] Search by city\n");
  printf("[2] Get total average\n");
  printf("[3] Exit\n");
  printf("\nChoose an option: ");
  scanf("%d", &op);
  return op;
}

int menu2() {
  int op;
  line();
  line2();
  printf("*  Isolation data research  *\n");
  line2();
  line();
  printf("\n[1] Search by city\n");
  printf("[2] Get total average\n");
  printf("[3] Exit\n");
  printf("[4] City average\n");
  printf("\nChoose an option: ");
  scanf("%d", &op);
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
