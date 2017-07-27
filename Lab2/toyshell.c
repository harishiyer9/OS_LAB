#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 64
#define DELIM " \t\r\n\a"

// Splits the input string according to DELIM
char **split_input(char *line)
{
  int bufsize = BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL,DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

// Built-in commands
char *builtin [] = {"cd","exit"};

// To read input.
char *read_input(void)
{
    char *input = NULL;
    ssize_t size = 0;
    getline(&input,&size,stdin);
    return input;
}


// Function which executes the linux commands
int execute_ext(char ** args)
{
    pid_t pid,wpid;
    int status;

    pid = fork();
    if(pid == 0)
    {
        if(execvp(args[0],args) == -1)
        {
            perror("Cannot execute the command ");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0)
    {
        perror("Cannot execute the command");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}


// Function which decides whether the command is built in or not and executes it.
int execute(char ** args)
{
    int i;
    if(args[0] == NULL)
    {
        return 1;
    }
    for(i=0; i<2; i++)
    {
        if(strcmp(args[0],builtin[i])== 0)
        {
            if(strcmp(args[0],"cd") == 0)
            {
                if(args[1] == NULL)
                {
                    printf("Expected argument for \"cd\"\n");
                }
                else if(strcmp(args[0],"cd") == 0 && args[1] != NULL)
                {
                    int check = chdir(args[1]);
                    printf("Val of check%d",check);
                    if(check != 0)
                    {
                        perror("Error\n");
                    }
                }
            }
            if(strcmp(args[0],"exit") == 0)
            {
                return 0;
            }
            return 1;
        }
    }
    return execute_ext(args);
}

// Function which conatins infinite while loop which accepts user input and calls functions to execute them.
void toy_shell_loop()
{
    char *input;
    char **args;
    int status = 1;

    while(status)
    {
        printf("toyshell $>> ");
        input = read_input();
        args = split_input(input);
        int i=0;
	
        status = execute(args);

        free(input);
        free(args);
    }
}


int main()
{
    toy_shell_loop();
    return 0;
}

