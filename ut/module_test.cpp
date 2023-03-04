#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


TEST_GROUP(FirstTestGroup)
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

IGNORE_TEST(FirstTestGroup, FirstTest)
{
   FAIL("Fail me!");
}

TEST(FirstTestGroup, SecondTest)
{
   CHECK(true);
}
