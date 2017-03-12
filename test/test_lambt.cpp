#include <lambt/lambt.h>

#include <thread>
#include <gtest/gtest.h>

TEST(LambtTest, LambtMacroTest)
{
  ASSERT_NO_THROW(LAMBT_TIC("sleep0"););

  std::this_thread::sleep_for(lambt::milliseconds(1));

  ASSERT_NO_THROW(LAMBT_TOC("sleep0"););

  ASSERT_NO_THROW(LAMBT_TIC("sleep1"););

  std::this_thread::sleep_for(lambt::milliseconds(2));

  ASSERT_NO_THROW(LAMBT_TOC("sleep1"););

  ASSERT_NO_THROW(LAMBT_TICTOC(std::this_thread::sleep_for(lambt::milliseconds(1));););
}

int main (int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

