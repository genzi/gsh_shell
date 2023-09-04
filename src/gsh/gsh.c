#include "gsh.h"

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>


#define GSHRC_FILENAME  ".gshrc"


static GshStatus_t handleGshRc(void) {
    char gshrcPath[PATH_MAX];
    FILE *gshrc = NULL;
    char *homeDir = getenv("HOME");

    if(NULL == homeDir) {
        fprintf(stderr, "unknown home dir\n");
        return GshStatus_Error;
    }

    strncpy(gshrcPath, homeDir, PATH_MAX - strlen(GSHRC_FILENAME) - 2);
    strcat(gshrcPath, "/");
    strncat(gshrcPath, GSHRC_FILENAME, PATH_MAX - 2);

    printf("%s", gshrcPath);

    if(!(gshrc = fopen(gshrcPath, "r"))) {
        fprintf(stderr, "cannot open .gshrc\n");
        return GshStatus_Error;
    }

    while(Command_GetAndExecute(gshrc) != CommandStatus_EOF);

    fclose(gshrc);
    return GshStatus_OK;
}

static GshStatus_t init(void) {
    return handleGshRc();
}

static void printPrompt(void) {
    char *prompt = "(gsh) $ ";

    printf("%s", prompt);
}

GshStatus_t GshRun(void) {
    init();

    while(true) {
        printPrompt();

        switch(Command_GetAndExecute(stdin)) {
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
