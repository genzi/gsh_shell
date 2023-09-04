#ifndef __EXEC_H__
#define __EXEC_H__

typedef enum {
    ExecStatus_Error       = -1,
    ExecStatus_OK          = 0,
    ExecStatus_NotFound    = 1,
    ExecStatus_NullCmd     = 2
} ExecStatus_t;

char *Exec_GetAliasCommand(char *alias);
ExecStatus_t Exec_CallInternal(char **command);
void Exec_PrintInternal(void);
ExecStatus_t Exec_CallExternal(char **command);

#endif  // end __EXEC_H__
