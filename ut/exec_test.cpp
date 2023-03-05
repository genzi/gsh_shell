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
