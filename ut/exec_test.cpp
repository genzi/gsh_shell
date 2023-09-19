#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

extern "C" {
#include "gsh/exec.h"
}

TEST_GROUP(ExecTestGroup)
{
   void setup()
   {
      // Init stuff
   }

   void teardown()
   {
      mock().checkExpectations();
      mock().clear();
   }
};

TEST(ExecTestGroup, Exec_GetAliasCommand_UnknownAlias_ReturnNull)
{
   char alias[] = {"UnknownAlias"};
   char *command;

   command = Exec_GetAliasCommand(alias);

   POINTERS_EQUAL(command, NULL);
}

TEST(ExecTestGroup, Exec_GetAliasCommand_llAlias_ReturnCommand)
{
   char alias[] = {"ll"};
   char command[] = {"ls -alF"};
   char *returnedCommand;

   Exec_AddAliasCommand(alias, command);
   returnedCommand = Exec_GetAliasCommand(alias);

   STRCMP_EQUAL("ls -alF", returnedCommand);
}

TEST(ExecTestGroup, Exec_TestAliasesCommands)
{
   char alias[] = {"alias1"};
   char command[] = {"command1"};
   char *returnedCommand;
   ExecStatus_t status;

   Exec_AddAliasCommand(alias, command);

   strcpy(alias, "alias2");
   strcpy(command, "command2");
   Exec_AddAliasCommand(alias, command);

   strcpy(alias, "alias3");
   strcpy(command, "command3");
   Exec_AddAliasCommand(alias, command);

   returnedCommand = Exec_GetAliasCommand(alias);
   STRCMP_EQUAL("command3", returnedCommand);

   strcpy(alias, "alias1");
   returnedCommand = Exec_GetAliasCommand(alias);
   STRCMP_EQUAL("command1", returnedCommand);

   strcpy(alias, "alias2");
   returnedCommand = Exec_GetAliasCommand(alias);
   STRCMP_EQUAL("command2", returnedCommand);

   status = Exec_DelAliasCommand(NULL);
   CHECK_EQUAL(ExecStatus_Error, status);

   status = Exec_DelAliasCommand(alias);
   CHECK_EQUAL(ExecStatus_OK, status);

   returnedCommand = Exec_GetAliasCommand(alias);
   POINTERS_EQUAL(returnedCommand, NULL);

   strcpy(alias, "alias3");
   returnedCommand = Exec_GetAliasCommand(alias);
   STRCMP_EQUAL("command3", returnedCommand);

}

TEST(ExecTestGroup, Exec_CallInternal_Unknown_ReturnNotFound)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"UnknownCommand" };
   commandPtr[1] = { (char *)"UnknownCommand" };
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_OK;

   status = Exec_CallInternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_NotFound, status);
}

TEST(ExecTestGroup, Exec_CallInternal_Help_ReturnOk)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"help" };
   commandPtr[1] = { (char *)"help" };
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_NotFound;

   status = Exec_CallInternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_OK, status);
}

TEST(ExecTestGroup, Exec_CallInternal_Echo_ReturnOk)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"echo" };
   commandPtr[1] = { (char *)"Hello World!" };
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_NotFound;

   status = Exec_CallInternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_OK, status);
}

TEST(ExecTestGroup, Exec_CallInternal_Cd_ReturnOk)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"cd" };
   commandPtr[1] = NULL;
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_NotFound;

   status = Exec_CallInternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_OK, status);
}

TEST(ExecTestGroup, Exec_CallInternal_Exit_ReturnOk)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"exit" };
   commandPtr[1] = { (char *)"exit" };
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_NotFound;

   status = Exec_CallInternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_OK, status);
}

TEST(ExecTestGroup, Exec_CallInternal_CommandNull_ReturnNullCmd)
{
   char *commandPtr[3] = {NULL};

   ExecStatus_t status = ExecStatus_OK;

   status = Exec_CallInternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_NullCmd, status);

   status = Exec_CallInternal(NULL);

   CHECK_EQUAL(ExecStatus_Error, status);
}

TEST(ExecTestGroup, Exec_CallExternal_ls_ReturnOk)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"ls" };
   commandPtr[1] = NULL;
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_NotFound;

   status = Exec_CallExternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_OK, status);
}

TEST(ExecTestGroup, Exec_CallExternal_unknown_ReturnNotFound)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"unknown" };
   commandPtr[1] = NULL;
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_OK;

   status = Exec_CallExternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_NotFound, status);
}

TEST(ExecTestGroup, Exec_CallExternal_pwd_ReturnOk)
{
   char *commandPtr[3];

   commandPtr[0] = { (char *)"pwd" };
   commandPtr[1] = NULL;
   commandPtr[2] = NULL;

   ExecStatus_t status = ExecStatus_NotFound;

   status = Exec_CallExternal((char **)commandPtr);

   CHECK_EQUAL(ExecStatus_OK, status);
}
