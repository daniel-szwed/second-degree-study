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

// Returns user input as number and set result to 1 if something went wrong
double askUserForNumber(char* message) {
   int result = -1;
   double userNumber = 0.0;
   while (result != 0) {
      errno = 0;
      if (signalCode == SIGINT) {
         printf("Przerwano przez uzytkownika.\n");
         return 0;
      }
      int userInputMaxSize = 1024;
      char userInput[userInputMaxSize];
      clearArray(userInput, userInputMaxSize);
      printf("%s\n", message);
      fillArrayWithUserInput(userInput, userInputMaxSize);
      char *endptr = NULL;
      userNumber = strtod(userInput, &endptr);
      if (userNumber == 0 && (errno != 0 || endptr == userInput)) {
         printf("Wprowadzono bledne dane (dozwolone sa tylko liczby w formacie dziesietnym)\n");
      } else {
         result = 0;
      }
   }   
   return userNumber;
}

int main(int argc, char *argv[]) {
   signal(SIGINT, signalHandler);
   int userInputMaxSize = 1024;
   char userInput[userInputMaxSize];
   for(;;) {
      errno = 0;
      if (signalCode == SIGINT) {
         printf("Przerwano przez uzytkownika.\n");
         return 0;
      }
      double firstOperand = askUserForNumber("Podaj pierwsza liczbe:");
      printf("Podaj rodzaj dzialania (wpisz *, /, + lub -):\n");
      clearArray(userInput, userInputMaxSize);
      fillArrayWithUserInput(userInput, userInputMaxSize);
      double secondOperand = askUserForNumber("Podaj druga liczbe:");
      FILE *pFile;
      pFile=fopen("wynik.txt", "a");
      switch (userInput[0])
      {
      case '*':
         printf("%lf * %lf = %lf\n\n", firstOperand, secondOperand, firstOperand * secondOperand);
         fprintf(pFile, "%lf * %lf = %lf\n", firstOperand, secondOperand, firstOperand * secondOperand);
         break;
      case '/':
         printf("%lf / %lf = %lf\n\n", firstOperand, secondOperand, firstOperand / secondOperand);
         fprintf(pFile, "%lf / %lf = %lf\n", firstOperand, secondOperand, firstOperand / secondOperand);
         break;
      case '+':
         printf("%lf + %lf = %lf\n\n", firstOperand, secondOperand, firstOperand + secondOperand);
         fprintf(pFile, "%lf + %lf = %lf\n", firstOperand, secondOperand, firstOperand + secondOperand);
         break;
      case '-':
         printf("%lf - %lf = %lf\n\n", firstOperand, secondOperand, firstOperand - secondOperand);
         fprintf(pFile, "%lf - %lf = %lf\n", firstOperand, secondOperand, firstOperand - secondOperand);
         break;               
      default:
         printf("Podany operator jest nieprawidlowy.\n");
         break;
      }
      fclose(pFile);
   }
   return 0;
}