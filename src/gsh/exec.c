#include "exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>


#define EXEC_INTERNAL_HELP  "help"
#define EXEC_INTERNAL_EXIT  "exit"
#define EXEC_INTERNAL_ECHO  "echo"
#define EXEC_INTERNAL_CD    "cd"
#define EXEC_INTERNAL_CRASHME "crashme"


typedef struct {
    char *alias;
    char *command;
} ExecAlias_t;

typedef struct {
    char *command;
    void *(*callback)(void *);
} ExecInternal_t;


static void *help_Callback(void *_ptr);
static void *exit_Callback(void *_ptr);
static void *echo_Callback(void *_ptr);
static void *cd_Callback(void *_ptr);
static void *crashme_Callback(void *_ptr);


static ExecAlias_t aliasesTable[] = {
    {"ll", "ls -alF"},
    {NULL, NULL},
};

static ExecInternal_t commandsTable[] = {
    [0].command     = EXEC_INTERNAL_HELP,
    [0].callback    = help_Callback,
    [1].command     = EXEC_INTERNAL_EXIT,
    [1].callback    = exit_Callback,
    [2].command     = EXEC_INTERNAL_ECHO,
    [2].callback    = echo_Callback,
    [3].command     = EXEC_INTERNAL_CD,
    [3].callback    = cd_Callback,
    [4].command     = EXEC_INTERNAL_CRASHME,
    [4].callback    = crashme_Callback,
    [5].command     = NULL,
    [5].callback    = NULL
};


static void *help_Callback(void *_ptr) {
    (void)_ptr;

    printf("Supported commands: ");
    Exec_PrintInternal();

    return NULL;
}

static void *exit_Callback(void *_ptr) {
    (void)_ptr;

    fclose(stdin);

    return NULL;
}

static void *echo_Callback(void *_ptr) {
    char **args = (char **)_ptr;

    for(int i = 1; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
    return NULL;
}

char lastPath[PATH_MAX];

static void *cd_Callback(void *_ptr) {
    char **args = (char **)_ptr;
    char *dir = args[1];
    char currentPath[PATH_MAX];

    if(getcwd(currentPath, sizeof(currentPath)) == NULL) {
        *currentPath = '\0';
    }

    if(NULL == dir || !strcmp(dir, "~")) {
        dir = getenv("HOME");
        if(NULL == dir) {
            fprintf(stderr, "Unknown home dir");
            return NULL;
        }
    }

    if(!strcmp(dir, "-")) {
        if(*lastPath == '\0') {
            fprintf(stderr, "No last directory!\n");
            return NULL;
        }
        dir = lastPath;
    }

    if(-1 == chdir(dir)) {
        perror("cd");
        return NULL;
    }

    strncpy(lastPath, currentPath, PATH_MAX);

    return NULL;
}

static void *crashme_Callback(void *_ptr) {
    (void)_ptr;

    u_int32_t * carshPtr = (u_int32_t *)0x01;
    *carshPtr = 0xDEAD;

    return NULL;
}

ExecStatus_t Exec_CallInternal(char **command) {

    ExecStatus_t status = ExecStatus_NotFound;

    if(command == NULL) {
        return ExecStatus_Error;
    }

    if(command[0] == NULL) {
        return ExecStatus_NullCmd;
    }

    for(int i = 0; commandsTable[i].command != NULL; i++) {
        if(!strcmp(commandsTable[i].command, command[0])) {
            if(NULL == commandsTable[i].callback) {
                status = ExecStatus_Error;
                break;
            } else {
                commandsTable[i].callback(command);
                status = ExecStatus_OK;
                break;
            }
        }
    }

    return status;
}

char *Exec_GetAliasCommand(char *alias) {
    int i = 0;

    alias[strcspn(alias, "\n")] = 0;

    while (aliasesTable[i].alias != NULL) {
        if (strcmp(alias, aliasesTable[i].alias) == 0) {
            return(aliasesTable[i].command);
        }
        i++;
    }
    return NULL;
}

void Exec_PrintInternal(void) {

    for(int i = 0; commandsTable[i].command != NULL; i++) {
        printf("%s ", commandsTable[i].command);
    }
    printf("\n");
}

static char *getCommandPath(char *command) {
    char *path, *pathCopy, *pathToken, *filePath;
    int commandLength, directoryLength;
    struct stat buffer;

    path = getenv("PATH");

    if(path) {
        pathCopy = strdup(path);
        commandLength = strlen(command);

        pathToken = strtok(pathCopy, ":");
        while(pathToken != NULL){
            directoryLength = strlen(pathToken);
            filePath = malloc(commandLength + directoryLength + 2);

            strcpy(filePath, pathToken);
            strcat(filePath, "/");
            strcat(filePath, command);
            strcat(filePath, "\0");

            if(stat(filePath, &buffer) == 0){
                free(pathCopy);
                return (filePath);
            } else {
                free(filePath);
                pathToken = strtok(NULL, ":");
            }
        }
        free(pathCopy);
        if (stat(command, &buffer) == 0) {
            return (command);
        }
        return (NULL);
    }
    return (NULL);
}

ExecStatus_t Exec_CallExternal(char **command) {
    ExecStatus_t status = ExecStatus_OK;
    char *commandFullPath = getCommandPath(command[0]);

    if(NULL == commandFullPath) {
        return ExecStatus_NotFound;
    }

    if(fork() == 0) {
        if (execve(commandFullPath, command, NULL) == -1) {
            perror("Exec error");
            status = ExecStatus_Error;
        }
    } else {
        while(wait(NULL) > 0);
    }
    free(commandFullPath);
    return status;
}
