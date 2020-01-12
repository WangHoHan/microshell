#define ARGUMENTS_MAX 1024
#define BASH_HISTORY_DIRECTORY "/home/students/s458903/history.txt"
#define COMMAND_MAX 1024
#define TRUE 1
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
void parse_command(char * arguments[], char command[])
{
    char * token = strtok(command, " ");
    int i = 0;
    while (token != NULL)
    {
        arguments[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    arguments[i] = NULL;
}
void read_command(char command[], FILE * fptr)
{
    fgets(command, COMMAND_MAX, stdin);
    fptr = fopen(BASH_HISTORY_DIRECTORY, "a");
    if (fptr == NULL)
    {
         perror("file not found");
         exit(EXIT_FAILURE);
    }
    else
    {
        fputs(command, fptr);
        fclose(fptr);
        command[strcspn(command, "\r\n")] = 0;
    }
}
void type_prompt()
{
    char path[PATH_MAX];
    getcwd(path, sizeof(path));
    printf("[");
    printf("\033[01;33m");
    printf("%s", getenv("USER"));
    printf("\033[0m");
    printf(":");
    printf("\033[1;31m");
    if (strcmp(getenv("HOME"), path) == 0)
    {
        printf("%c", '~');
    }
    else
    {
        printf("%s", path);
    }
    printf("\033[0m");
    printf("] $ ");
}
int main(int argc, char * argv[])
{
    char * arguments[ARGUMENTS_MAX];
    char command[COMMAND_MAX];
    FILE * fptr = NULL;
    pid_t pid;
    time_t t;
    while (TRUE)
    {
        type_prompt();
        read_command(command, fptr);
        parse_command(arguments, command);
        if (strcmp("cd", arguments[0]) == 0)
        {
            if (arguments[1] == NULL)
            {
                chdir(getenv("HOME"));
            }
            else if (strcmp("~", arguments[1]) == 0)
            {
                chdir(getenv("HOME"));
            }
            else
            {
                if (chdir(arguments[1]) == -1)
                {
                    perror("directory not found");
                }
            }
        }
        else if (strcmp("cp", arguments[0]) == 0)
        {
            char c;
            FILE * fptr1 = NULL;
            FILE * fptr2 = NULL;
            fptr1 = fopen(arguments[1], "r");
            if (fptr1 == NULL)
            {
                perror("file not found");
                exit(EXIT_FAILURE);
            }
            else
            {
                fptr2 = fopen(arguments[2], "a");
                if (fptr2 == NULL)
                {
                    fclose(fptr1);
                    perror("file not found");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    c = fgetc(fptr1);
                    while (c != EOF)
                    {
                        fputs(&c, fptr2);
                        c = fgetc(fptr1);
                    }
                    fclose(fptr1);
                    fclose(fptr2);
                }
            }
        }
        else if (strcmp("date", arguments[0]) == 0)
        {
            time(&t);
            printf("%s", ctime(&t));
        }
        else if (strcmp("exit", arguments[0]) == 0)
        {
            exit(0);
        }
        else if (strcmp("help", arguments[0]) == 0)
        {
            printf("microshell\n");
            printf("Tomasz Adamczyk\n");
        }
        else if (strcmp("history", arguments[0]) == 0)
        {
            char c;
            fptr = fopen(BASH_HISTORY_DIRECTORY, "r");
            if (fptr == NULL)
            {
                perror("file not found");
                exit(EXIT_FAILURE);
            }
            else
            {
                c = fgetc(fptr);
                while (c != EOF)
                {
                    printf("%c", c);
                    c = fgetc(fptr);
                }
                fclose(fptr);
            }
        }
        else if (strcmp("mv", arguments[0]) == 0)
        {
            char c;
            FILE * fptr1 = NULL;
            FILE * fptr2 = NULL;
            fptr1 = fopen(arguments[1], "r");
            if (fptr1 == NULL)
            {
                perror("file not found");
                exit(EXIT_FAILURE);
            }
            else
            {
                fptr2 = fopen(arguments[2], "a");
                if (fptr2 == NULL)
                {
                    fclose(fptr1);
                    perror("file not found");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    c = fgetc(fptr1);
                    while (c != EOF)
                    {
                        fputs(&c, fptr2);
                        c = fgetc(fptr1);
                    }
                    fclose(fptr1);
                    fclose(fptr2);
                    if (remove(arguments[1]) == -1)
                    {
                        perror("file not found");
                    }
                }
            }
        }
        else if (strcmp("rm", arguments[0]) == 0)
        {
            if (remove(arguments[1]) == -1)
            {
                perror("file not found");
            }
        }
        else
        {
            pid = fork();
            if (pid != 0)
            {
                waitpid(-1, NULL, 0);
            }
            else
            {
                execvp(arguments[0], arguments);
                perror("command not found");
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}
