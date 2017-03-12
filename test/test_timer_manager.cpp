#include <lambt/timer_manager.h>

#include <thread>
#include <gtest/gtest.h>

TEST(LambtTest, TimerManagerTest)
{
  lambt::TimerManager<lambt::Timer, lambt::default_precision, std::string> timer_manager{"sleep0", "sleep1"};

  ASSERT_NO_THROW(timer_manager.tic("sleep0"););

  std::this_thread::sleep_for(lambt::milliseconds(1));

  lambt::TimerU::Duration sleep0_toc;
  ASSERT_NO_THROW(sleep0_toc = timer_manager.toc("sleep0"););

  ASSERT_GT(sleep0_toc.count(), 999.999);

  ASSERT_NO_THROW(timer_manager.tic("sleep1"););

  std::this_thread::sleep_for(lambt::milliseconds(2));

  lambt::TimerU::Duration sleep1_toc;
  ASSERT_NO_THROW(sleep1_toc = timer_manager.toc("sleep1"););

  ASSERT_GT(sleep1_toc.count(), 1999.999);

  ASSERT_NO_THROW(timer_manager.tic("sleep2"););

  std::this_thread::sleep_for(lambt::milliseconds(1));

  lambt::TimerU::Duration sleep2_toc;
  ASSERT_NO_THROW(sleep2_toc = timer_manager.toc("sleep2"););

  ASSERT_GT(sleep0_toc.count(), 999.999);

  ASSERT_EQ(timer_manager.last_elapsed("sleep2"), sleep2_toc);

  const auto key_list = timer_manager.keys();

  const auto expected_key_list = std::vector<std::string>{"sleep0","sleep1","sleep2"};

  ASSERT_EQ(key_list, expected_key_list);

  const auto reports = timer_manager.reports();

  ASSERT_TRUE(reports.size() == 3);

  timer_manager.clear();

  ASSERT_TRUE(timer_manager.keys().empty());
}

int main (int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

