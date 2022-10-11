#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static volatile int signalCode = 0;

void signalHandler(int signal) {
    signalCode = signal;
}

void clearArray(char* array, int size) {
   int i;
   for (i=0; i<size; i++) {
      *array = '\0';
      array++;
   }
}

void fillArrayWithUserInput(char* array, int size) {
   int i;
   for (i=0; i<size; i++) {
      if (signalCode == SIGINT) {
         return;
      }
      int input = getchar();
      if (input == '\n') {
         return;
      }
      *array = input;
      array++;
   }
}

int main(int argc, char *argv[]) {
   signal(SIGINT, signalHandler);
   int userInputMaxSize = 1024;
   char userInput[userInputMaxSize];
   for(;;) {
      errno = 0;
      clearArray(userInput, userInputMaxSize);
      printf("Podaj liczbe i zatwierdz klawiszem enter:\n");
      fillArrayWithUserInput(userInput, userInputMaxSize);
      if (signalCode == SIGINT) {
         printf("Przerwano przez uzytkownika.\n");
         return 0;
      }
      char *endptr = NULL;
      double userNumber = strtod(userInput, &endptr);
      if (userNumber == 0 && (errno != 0 || endptr == userInput)) {
         printf("Wprowadzono bledne dane (dozwolone sa tylko liczby w formacie dziesietnym)\n");
         continue;
      }
      if (userNumber > 0.0) {
         printf("Podana liczba jest wieksza od zera.\n");
      } else if (userNumber == 0.0) {
         printf("Podana liczba to zero");
      } else { 
         printf("Podana liczba jest mniejsza od zera.\n");
      }
   }
   return 0;
}