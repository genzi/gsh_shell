#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <stdlib.h>


typedef enum {
    CommandStatus_Error = -1,
    CommandStatus_OK    = 0,
    CommandStatus_EOF   = 1
} CommandStatus_t;

CommandStatus_t Command_GetAndExecute(void);


#endif  // end __COMMAND_H__
