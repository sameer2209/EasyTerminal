#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
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

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

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

  inputCommandString = trimwhitespace(inputCommandString);

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
  //free(tempInputString);
  //free(inputCommandString);
  return tempInputString;
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

void execShowCommand(){
  //printf("hello");
  pid_t pid;
  pid = fork();
  if(pid == 0){
    //printf("inside fork");
    char command[50];

    if(strcmp(structuredInputCommand.arguements[0], "files") == 0){
      strcpy(command, "ls -l | grep \"^-\"");
    }

    else if(strcmp(structuredInputCommand.arguements[0], "folders") == 0){
      strcpy(command, "ls -l | grep \"^d\"");
    }

    else if(strcmp(structuredInputCommand.arguements[0], "all") == 0){
      strcpy(command, "ls -l");
    }

    else{
      printf("INVALID COMMAND");
    }

    system(command);
  }

  else if(pid > 0){
    int status;
    wait(&status);
  }

  else{
    printf("PROCESS CANNOT BE CREATED");
  }
}

int getLastIndexOfSlash(char* currentDirectory){

  int length = strlen(currentDirectory);
  int i, lastIndexOfSlash;

  for(i = length-1; i>=0; i--){
    if(currentDirectory[i] == '/'){
      lastIndexOfSlash = i;
    }
  }
  return lastIndexOfSlash;
}

void execGoCommand(){

  //printf("inside execGoCommand");
  puts("inside execGoCommand");

  if(no_of_command_tokens == 2){
    if(strcmp(structuredInputCommand.option, "back") == 0){
      puts("inside A");
      char* currentDirectory;
      currentDirectory = (char*)malloc(500*sizeof(char));
      getcwd(currentDirectory, (size_t)500);
      //printf("%s", currentDirectory);
      puts(currentDirectory);
      char* parentDirectory;
      int lastIndexOfSlash,i;
      lastIndexOfSlash = getLastIndexOfSlash(currentDirectory);
      parentDirectory = (char*)malloc(200*sizeof(char));

      for(i=0; i<=lastIndexOfSlash; i++){
        parentDirectory[i] = currentDirectory[i];
      }

      chdir(parentDirectory);
    }
    else{
      printf("INVALID COMMAND");
    }
  }

  if(no_of_command_tokens == 4){
    if(strcmp(structuredInputCommand.option, "directory") == 0){
      chdir(structuredInputCommand.arguements[0]);
    }
    else{
      printf("INVALID COMMAND");
    }
  }

  //displayCurrentDirectory();

  // char command[50];
  // if(no_of_command_tokens == 2){
  //   strcpy(command, "cd ..");
  //   system(command);
  //   displayCurrentDirectory();
  // }
  //
  // if(no_of_command_tokens == 4){
  //   if(strcmp(structuredInputCommand.option, "directory") == 0 && strcmp(structuredInputCommand.option, "to") == 0){
  //     char directoryName[100];
  //     strcpy(directoryName, structuredInputCommand.arguements[0]);
  //     strcpy(command, "cd ");
  //     strcat(command, directoryName);
  //     system(command);
  //     displayCurrentDirectory();
  //
  //   }
  //   else{
  //     printf("INVALID COMMAND");
  //   }
  //
  // }

}
void execCreateCommand(){

  pid_t pid;
  pid = fork();
  if(pid == 0){
    //printf("inside fork");
    char command[50];
    char filename[50];
    strcpy(filename, structuredInputCommand.arguements[0]);


    if(strcmp(structuredInputCommand.option, "file") == 0){
      strcpy(command, "touch ");
    }

    else if(strcmp(structuredInputCommand.option, "folder") == 0){
      //strcpy(command, "ls -l | grep \"^d\"");
      strcpy(command, "mkdir ");
    }

    else{
      printf("INVALID COMMAND");
    }

    strcat(command, filename);
    system(command);
  }

  else if(pid > 0){
    int status;
    wait(&status);
  }

  else{
    printf("PROCESS CANNOT BE CREATED");
  }
}

void execDeleteCommand(){

  pid_t pid;
  pid = fork();
  if(pid == 0){
    //printf("inside fork");
    char command[50];
    char filename[50];
    strcpy(filename, structuredInputCommand.arguements[0]);


    if(strcmp(structuredInputCommand.option, "file") == 0){
      strcpy(command, "rm ");
    }

    else if(strcmp(structuredInputCommand.option, "folder") == 0){
      //strcpy(command, "ls -l | grep \"^d\"");
      strcpy(command, "rmdir ");
    }

    else{
      printf("INVALID COMMAND");
    }

    strcat(command, filename);
    system(command);
  }

  else if(pid > 0){
    int status;
    wait(&status);
  }

  else{
    printf("PROCESS CANNOT BE CREATED");
  }

}

void execCopyCommand(){

  pid_t pid;
  pid = fork();
  if(pid == 0){
    //printf("inside fork");
    char command[200];
    char source[100];
    char destination[100];
    strcpy(source, structuredInputCommand.arguements[0]);
    strcpy(destination, structuredInputCommand.arguements[1]);

    strcpy(command, "cp ");

    strcat(command, source);
    strcat(command, " ");
    strcat(command, destination);
    system(command);
  }

  else if(pid > 0){
    int status;
    wait(&status);
  }

  else{
    printf("PROCESS CANNOT BE CREATED");
  }

}

void execMoveCommand(){

  pid_t pid;
  pid = fork();
  if(pid == 0){
    //printf("inside fork");
    char command[200];
    char source[100];
    char destination[100];
    strcpy(source, structuredInputCommand.arguements[0]);
    strcpy(destination, structuredInputCommand.arguements[1]);

    strcpy(command, "mv ");

    strcat(command, source);
    strcat(command, " ");
    strcat(command, destination);
    system(command);
  }

  else if(pid > 0){
    int status;
    wait(&status);
  }

  else{
    printf("PROCESS CANNOT BE CREATED");
  }

}

void execRenameCommand(){

}

void execCommand(){

  if(no_of_command_tokens == 2){
    //printf("inside block");
    puts("inside block");
    if(strcmp(structuredInputCommand.name, "show") == 0){
      execShowCommand();
    }

    else if(strcmp(structuredInputCommand.name, "go") == 0){
      //printf("inside execCommand");
      puts("inside execCommand");
      execGoCommand();
    }

    else{
      printf("COMMAND NOT SUPPORTED\n");
      return;
    }
  }

  else if (no_of_command_tokens == 3) {

    if(strcmp(structuredInputCommand.name, "show") == 0 && strcmp(structuredInputCommand.arguements[0], "directory") == 0 && strcmp(structuredInputCommand.option, "current") == 0){
      displayCurrentDirectory();
    }

    else if(strcmp(structuredInputCommand.name, "create") == 0){
      execCreateCommand();
    }
    else if(strcmp(structuredInputCommand.name, "delete") == 0){
      execDeleteCommand();
    }
    else{
      printf("COMMAND NOT SUPPORTED\n");
    }
  }

  else if(no_of_command_tokens == 4){
    if(strcmp(structuredInputCommand.name, "go") == 0){
      execGoCommand();
    }
    else if(strcmp(structuredInputCommand.name, "rename") == 0){
      execRenameCommand();
    }
    else if(strcmp(structuredInputCommand.name, "copy") == 0){
      execCopyCommand();
    }
    else if(strcmp(structuredInputCommand.name, "move") == 0){
      execMoveCommand();
    }
    else{
      printf("COMMAND NOT SUPPORTED\n");
    }
  }

  return;
}

int main(int argc, char const *argv[]) {

  char* tempInputString = (char*)malloc(200*sizeof(char));
  displayCurrentDirectory();
  tempInputString = getCommandInput();
  printf("tempInputString is: %s\n",tempInputString);
  //printf("%s",tempInputString);

  while(strcmp(tempInputString, "exit") != 0){
    if(no_of_command_tokens < 2 || no_of_command_tokens > 4){
      printf("INVALID COMMAND\n");
    }

    else{
      parseInputCommand();
      //printf("inside else");
      puts("inside else");
      execCommand();
    }
    //printf("%p\n", inputCommandString);
    displayCurrentDirectory();
    //printf("%p\n", inputCommandString);
    free(inputCommandString);
    //free(tempInputString);
    //inputCommandString = NULL;
    tempInputString = getCommandInput();
  }

  return 0;
}
