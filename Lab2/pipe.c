#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#define READ  0
#define WRITE 1


//This is used to parse the string
#define DELIM " \t\r\n\a"
#define READ  0
#define WRITE 1
static char* args[512];
pid_t pid;

/*
 * Handle commands separatly
 * input: return value from previous command (useful for pipe file descriptor)
 * first: 1 if first command in pipe-sequence (no input from previous pipe)
 * last: 1 if last command in pipe-sequence (no input from previous pipe)
 */

// This is the main function for multiple pipelining.
static int command(int input, int first, int last)
{
    int pipettes[2];

    /* Invoke pipe */
    pipe( pipettes );
    pid = fork();
    if(pid<0)
    {
        perror("Cannot execute the command ");
    }

    if (pid == 0)
    {
        if (first == 1 && last == 0 && input == 0)
        {
            // First command
            dup2( pipettes[WRITE], STDOUT_FILENO );
        }
        else if (first == 0 && last == 0 && input != 0)
        {
            // Middle command
            dup2(input, STDIN_FILENO);
            dup2(pipettes[WRITE], STDOUT_FILENO);
        }
        else
        {
            // Last command
            dup2( input, STDIN_FILENO );
        }

        if (execvp( args[0], args) == -1)
        {
            perror("Cannot execute the command ");
            _exit(EXIT_FAILURE);
        }
    }

    if (input != 0)
        close(input);

    // Nothing more needs to be written
    close(pipettes[WRITE]);

    // If it's the last command, nothing more needs to be read
    if (last == 1)
        close(pipettes[READ]);

    return pipettes[READ];
}

/*  Waits for processes to terminate.
 *  n : Number of times 'command' was invoked.
 */
static void wait__(int n)
{
    int i;
    for (i = 0; i < n; ++i)
        wait(NULL);
}

int run(char* cmd, int input, int first, int last);
char line[1024];
static int n = 0; /* number of calls to 'command' */
//Skips white character
char* skipwhite(char* s)
{
    while (isspace(*s)) ++s;
    return s;
}


void split(char* cmd)
{
    cmd = skipwhite(cmd);
    char* next = strchr(cmd, ' ');
    int i = 0;

    while(next != NULL)
    {
        next[0] = '\0';
        args[i] = cmd;
        ++i;
        cmd = skipwhite(next + 1);
        next = strchr(cmd, ' ');
    }

    if (cmd[0] != '\0')
    {
        args[i] = cmd;
        next = strchr(cmd, '\n');
        next[0] = '\0';
        ++i;
    }

    args[i] = NULL;
}

int run(char* cmd, int input, int first, int last)
{
    split(cmd);
    if (args[0] != NULL)
    {
        n += 1;
        return command(input, first, last);
    }
    return 0;
}

// Used to parse the input string based on DELIM declared above
char **split_input(char *line)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    if (!tokens)
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line,DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        if (position >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL,DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
// Internal Commands of shell
char *builtin [] = {"cd","exit"};
// To read input
char *read_input(void)
{
    char *input = NULL;
    ssize_t size = 0;
    getline(&input,&size,stdin);
    return input;
}
// Executes the internal shell commmads.
int execute(char * line)
{
    char** args = split_input(line);
    int i;
    if(args[0] == NULL)
    {
        return 1;
    }
    for(i=0; i<2; i++)
    {
        if(strcmp(args[0],builtin[i])== 0)
        {
            if(strcmp(args[0],"cd") == 0) // Execution of cd
            {
                if(args[1] == NULL)
                {
                    printf("Expected argument for \"cd\"\n");
                }
                else if(strcmp(args[0],"cd") == 0 && args[1] != NULL)
                {
                    int check = chdir(args[1]);
                    if(check != 0)
                    {
                        perror("Error\n");
                    }
                }
            }
            if(strcmp(args[0],"exit") == 0) // Execution of exit
            {
                exit(0);
            }
            return 1;
        }
    }
}
// Function which has an infinite while loop to execte user arguments.
int toy_shell_loop()
{
    char *line;
    char **args;
    int status = 1,flag = 1,i;
    while (1)
    {
        flag = 1;
        /* Print the command prompt */
        printf("ToyShell $>>> ");
        fflush(NULL);
        /* Read a command line */
        line = read_input();
        if(strncmp(line,"cd", 2) == 0 || strcmp(line,"exit\n") == 0)
        {
            flag = 0;
        }
        if(flag == 1)
        {
            int input = 0;
            int first = 1;
            char* cmd = line;
            char* next = strchr(cmd, '|'); /* Find first '|' */
            while (next != NULL)
            {
                /* 'next' points to '|' */
                *next = '\0';
                input = run(cmd, input, first, 0);
                cmd = next + 1;
                next = strchr(cmd, '|'); /* Find next '|' */
                first = 0;
            }
            input = run(cmd, input, first, 1); // To execute the last command of piped commands.
            wait__(n);
            n = 0;
        }
        else 	// To execute cd and exit
        {
            execute(line);
        }
    }
}
int main()
{
    toy_shell_loop();
    return 0;
}