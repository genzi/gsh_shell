#include "exec_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define EXEC_INTERNAL_HELP "help"
#define EXEC_INTERNAL_EXIT "exit"
#define EXEC_INTERNAL_ECHO "echo"


typedef struct
{
    char *command;
    void *(*callback)(void *);
} ExecInternal_t;


static void *help_Callback(void *_ptr);
static void *exit_Callback(void *_ptr);
static void *echo_Callback(void *_ptr);


static ExecInternal_t commandsTable[] = {
    [0].command     = EXEC_INTERNAL_HELP,
    [0].callback    = help_Callback,
    [1].command     = EXEC_INTERNAL_EXIT,
    [1].callback    = exit_Callback,
    [2].command     = EXEC_INTERNAL_ECHO,
    [2].callback    = echo_Callback,
    [3].command     = NULL,
    [3].callback    = NULL
};


static void *help_Callback(void *_ptr) {
    (void)_ptr;

    printf("Supported comamnds: ");
    ExecInternal_PrintCommands();

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

ExecInternalStatus_t ExecInternal_CallCommand(char **command) {

    ExecInternalStatus_t status = ExecInternalStatus_NotFound;

    for(int i = 0; commandsTable[i].command != NULL; i++) {
        if(!strcmp(commandsTable[i].command, command[0])) {
            if(NULL == commandsTable[i].callback) {
                status = ExecInternalStatus_Error;
                break;
            } else {
                commandsTable[i].callback(command);
                status = ExecInternalStatus_OK;
                break;
            }
        }
    }

    return status;
}

void ExecInternal_PrintCommands(void) {

    for(int i = 0; commandsTable[i].command != NULL; i++) {
        printf("%s ", commandsTable[i].command);
    }
    printf("\n");
}
