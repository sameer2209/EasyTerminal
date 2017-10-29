#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#defiine COMMAND_SIZE 200; 

struct Command{
  char* name;
  char* option;
  char* arguements;
  char* helper;
  int no_of_arguements;
};

void displayCurrentDirectory();

int main(int argc, char const *argv[]) {
  char* command;
  displayCurrentDirectory();
  fgets(command, )
  return 0;
}

void displayCurrentDirectory(){
  char* currentDirectory;
  currentDirectory = (char*)malloc(200*sizeof(char));
  getcwd(currentDirectory, (size_t)200);
  fprintf(stdout, "%s>\n", currentDirectory);
  printf("%d\n", errno);
  //printf("%s>\t\n", currentDirectory);
}
