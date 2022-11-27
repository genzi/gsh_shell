#ifndef __EXEC_INTERNAL_H__
#define __EXEC_INTERNAL_H__

typedef enum {
    ExecInternalStatus_Error        = -1,
    ExecInternalStatus_OK           = 0,
    ExecInternalStatus_NotFound    = 1
} ExecInternalStatus_t;

ExecInternalStatus_t ExecInternal_CallCommand(char **command);
void ExecInternal_PrintCommands(void);

#endif  // end __EXEC_INTERNAL_H__
