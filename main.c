#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#define COMMAND_SIZE 200;

struct Command{
  char* name; //name of caommand
  char* option; //options passed tocommand
  char* arguements[2]; //array to store arguements passed to command
  char* helper; //helper words used to write command like "to" "from" etc
  int no_of_arguements; //
};

struct Command structuredInputCommand;
char* inputCommandString; //command inputted by user
char* commandTokensArray[5]; //maximum number of tokens is 10
char* commandDelimiter = " "; //delimiter used to tokenize input command
int no_of_command_tokens;

void displayCurrentDirectory(){

  char* currentDirectory;
  currentDirectory = (char*)malloc(200*sizeof(char));
  getcwd(currentDirectory, (size_t)200);
  fprintf(stdout, "%s>\n", currentDirectory);
  printf("%d\n", errno);
  //printf("%s>\t\n", currentDirectory);
}

char* getCommandInput(){

  char* commandToken; //token from command
  int i = 0;
  char* tempInputString;
  inputCommandString = NULL;
  no_of_command_tokens = 0;

  inputCommandString = (char*)malloc(200*sizeof(char));
  //scanf("%200[^\n]\n", inputCommandString);
  fgets(inputCommandString, 200, stdin);

  tempInputString = (char*)malloc(sizeof(inputCommandString));
  strcpy(tempInputString, inputCommandString);

  printf("input String: %s\n", inputCommandString);
  printf("temp string: %s\n", tempInputString);
  commandToken = strtok(inputCommandString, commandDelimiter);

  while(commandToken != NULL){
    no_of_command_tokens++;
    if(i > 4){
      return inputCommandString;
    }
    commandTokensArray[i] = commandToken;
    commandToken = strtok(NULL, commandDelimiter);
    i++;
  }
  printf("no of command tokens: %d\n", no_of_command_tokens);
  printf("input string is: %s\n", inputCommandString);
  free(tempInputString);
  return inputCommandString;
  //return tempInputString;
}

void parseInputCommand(){

  int i = 0, j = 0;
  int optionFlag = 0;

  if(no_of_command_tokens == 2){
    structuredInputCommand.name = commandTokensArray[0];
    structuredInputCommand.arguements[0] = commandTokensArray[1];
  }
  else if (no_of_command_tokens == 3) {
    structuredInputCommand.name = commandTokensArray[0];
    structuredInputCommand.option = commandTokensArray[1];
    structuredInputCommand.arguements[0] = commandTokensArray[2];
  }
  else if (no_of_command_tokens == 4) {
    if(strcmp(commandTokensArray[0], "go") == 0){
      structuredInputCommand.name = commandTokensArray[0];
      structuredInputCommand.helper = commandTokensArray[1];
      structuredInputCommand.option = commandTokensArray[2];
      structuredInputCommand.arguements[0] = commandTokensArray[3];
    }
    else{
      structuredInputCommand.name = commandTokensArray[0];
      structuredInputCommand.helper = commandTokensArray[2];
      structuredInputCommand.arguements[0] = commandTokensArray[1];
      structuredInputCommand.arguements[1] = commandTokensArray[3];
    }
  }
  else if(no_of_command_tokens == 5){
    structuredInputCommand.name = commandTokensArray[0];
    structuredInputCommand.helper = commandTokensArray[1];
    structuredInputCommand.option = commandTokensArray[2];
    structuredInputCommand.arguements[0] = commandTokensArray[3];
    structuredInputCommand.arguements[1] = commandTokensArray[4];
  }

  printf("command name: %s\ncommand option: %s\narguement: %s\narguement: %s\nhelper: %s\n", structuredInputCommand.name, structuredInputCommand.option, structuredInputCommand.arguements[0], structuredInputCommand.arguements[1], structuredInputCommand.helper);
}

int main(int argc, char const *argv[]) {

  char* tempInputString = (char*)malloc(200*sizeof(char));
  displayCurrentDirectory();
  tempInputString = getCommandInput();
  printf("tempInputString is: %s\n", tempInputString);

  while(strcmp(tempInputString, "exit") != 0){
    if(no_of_command_tokens < 2 || no_of_command_tokens > 5){
      printf("INVALID COMMAND\n");
    }

    else{
      parseInputCommand();
    }

    displayCurrentDirectory();
    //printf("%p\n", inputCommandString);
    free(inputCommandString);
    //inputCommandString = NULL;
    tempInputString = getCommandInput();
  }

  return 0;
}
