#include "command.h"
#include "exec.h"

#include <stdio.h>
#include <string.h>


typedef enum {
    CommandType_Internal,
    CommandType_External
} CommandType_t;

typedef struct {
    CommandType_t type;
    char *command;
    int argc;
    char **argv;
} Command_t;



CommandStatus_t Command_GetAndExecute(FILE *input_stream) {

    char *aliasPtr = NULL;
    char *commandPtr = NULL;
    char *commandCopyPtr = NULL;
    const char *delimPtr = " \n";
    size_t commandLen = 0;
    ssize_t nbCharsRead = -1;
    ExecStatus_t status;

    nbCharsRead = getline(&commandPtr, &commandLen, input_stream);

    if(-1 == nbCharsRead) {
        free(commandPtr);
        return CommandStatus_EOF;
    }

    aliasPtr = Exec_GetAliasCommand(commandPtr);

    if(aliasPtr != NULL) {
        free(commandPtr);
        nbCharsRead = strlen(aliasPtr);
        commandPtr = malloc(sizeof(char) * (nbCharsRead+1));
        strcpy(commandPtr, aliasPtr);
    }

    commandCopyPtr = malloc(sizeof(char) * (nbCharsRead+1));
    if (commandCopyPtr == NULL) {
        perror("command: memory allocation error");
        free(commandPtr);
        return CommandStatus_Error;
    }
    strcpy(commandCopyPtr, commandPtr);

    char *token = strtok(commandPtr, delimPtr);
    int numTokens = 0;
    while (token != NULL) {
        numTokens++;
        token = strtok(NULL, delimPtr);
    }
    numTokens++;

    Command_t command;
    command.argc = numTokens;

    command.argv = malloc(sizeof(char *) * command.argc);

    token = strtok(commandCopyPtr, delimPtr);
    int i;
    for (i = 0; token != NULL; i++) {
        command.argv[i] = malloc(sizeof(char) * (strlen(token)+1));
        strcpy(command.argv[i], token);

        token = strtok(NULL, delimPtr);
    }
    command.argv[i] = NULL;

    if(aliasPtr == NULL) {
        switch(Exec_CallInternal(command.argv)) {
            case ExecStatus_OK:
                //Nothing to do here
                break;
            case ExecStatus_NotFound:
                status = Exec_CallExternal(command.argv);
                if(status == ExecStatus_Error) {
                    for(i = 0; i < command.argc; i++) {
                        free(command.argv[i]);
                    }
                    free(command.argv);
                    free(commandPtr);
                    free(commandCopyPtr);
                    exit(EXIT_FAILURE);
                } else if(status == ExecStatus_NotFound) {
                    fprintf(stderr, "Command \"%s\" not found!\n", command.argv[0]);
                }
                break;
            case ExecStatus_Error:
                fprintf(stderr, "Command execution error!\n");
                break;
            default:
                fprintf(stderr, "Unknown error!");
                break;
        }
    } else {
        status = Exec_CallExternal(command.argv);
        if(status == ExecStatus_Error) {
            for(i = 0; i < command.argc; i++) {
                free(command.argv[i]);
            }
            free(command.argv);
            free(commandPtr);
            free(commandCopyPtr);
            exit(EXIT_FAILURE);
        } else if(status == ExecStatus_NotFound) {
            fprintf(stderr, "Alias command \"%s\" not found!\n", command.argv[0]);
        } else {
            // Alias exec OK.
        }
    }

    for(i = 0; i < command.argc; i++) {
        free(command.argv[i]);
    }
    free(command.argv);
    free(commandPtr);
    free(commandCopyPtr);

    return CommandStatus_OK;
}

