#include "gsh.h"

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void printPrompt(void) {
    char *prompt = "(gsh) $ ";

    printf("%s", prompt);
}

GshStatus_t GshRun(void) {
    while(true) {
        printPrompt();

        switch(Command_GetAndExecute()) {
            case CommandStatus_EOF:
                printf("Exiting gsh shell...\n");
                exit(EXIT_SUCCESS);
            case CommandStatus_Error:
                fprintf(stderr, "Exiting gsh due to error.\n");
                exit(EXIT_FAILURE);
            case CommandStatus_OK:
                //nothing to do; just get and execute next command
                break;
        }

    }
    //unexpected error
    exit(EXIT_FAILURE);
}
