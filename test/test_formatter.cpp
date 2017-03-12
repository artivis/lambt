#include <lambt/timer_manager.h>
#include <lambt/formatter.h>

#include <thread>
#include <gtest/gtest.h>

TEST(LambtTest, ReportFormatterTest)
{
  const int avg_win = 500;
  lambt::DefaultAverageTimer avg_timer(avg_win);
  avg_timer.tic();

  std::this_thread::sleep_for(lambt::microseconds(1));

  lambt::DefaultAverageTimer::Duration first_toc;
  ASSERT_NO_THROW(first_toc = avg_timer.toc(););

  ASSERT_GT(first_toc.count(), 0);

  ASSERT_EQ(first_toc, avg_timer.last_elapsed());

  std::this_thread::sleep_for(lambt::microseconds(1));

  lambt::DefaultAverageTimer::Duration second_toc;
  ASSERT_NO_THROW(second_toc = avg_timer.toc(););

  ASSERT_GT(second_toc.count(), first_toc.count());

  ASSERT_EQ(avg_timer.calls(), 2);

  ASSERT_NO_THROW(avg_timer.reset(););

  lambt::DefaultAverageTimer::Duration avg_toc(0);
  ASSERT_NO_THROW(avg_toc = avg_timer.avg_toc(););

  ASSERT_EQ(avg_toc.count(), 0);
  ASSERT_EQ(avg_timer.calls(), 0);

  lambt::DefaultAverageTimer::Duration loop_toc;
  const int iterations = 501;
  for (int i=0; i<iterations; ++i)
  {
    avg_timer.tic();

    std::this_thread::sleep_for(lambt::microseconds(500));

    loop_toc = avg_timer.toc();
  }

  ASSERT_EQ(avg_timer.calls(), avg_win);

  ASSERT_NO_THROW(avg_toc = avg_timer.avg_toc(););

  EXPECT_GT(avg_toc.count(), lambt::DefaultAverageTimer::Duration(500).count());

  lambt::DefaultAverageTimer::Duration variance_toc(0);
  ASSERT_NO_THROW(variance_toc = avg_timer.variance_toc(););

  EXPECT_LT(variance_toc.count(), lambt::DefaultAverageTimer::Duration(250).count());

  lambt::DefaultAverageTimer::report_t report;
  ASSERT_NO_THROW(report = avg_timer.report(););

  ASSERT_EQ(report.calls, avg_win);
  ASSERT_EQ(report.last_elapsed, loop_toc);
  ASSERT_EQ(report.average, avg_toc);
  ASSERT_EQ(report.variance, variance_toc);

  std::string formated_report;
  ASSERT_NO_THROW(formated_report = lambt::ReportFormatter::format(report););

  std::cout << formated_report << std::endl;

  ASSERT_FALSE(formated_report.empty());
}

TEST(LambtTest, ReportsFormatterTest)
{
  lambt::TimerManager<lambt::AverageTimer, lambt::default_precision, std::string> timer_manager{"sleep0","sleep1","sleep2"};

  const int iterations = 500;
  for (int i=0; i<iterations; ++i)
  {
    ASSERT_NO_THROW(timer_manager.tic("sleep0"););

    std::this_thread::sleep_for(lambt::milliseconds(1));

    ASSERT_NO_THROW(timer_manager.toc("sleep0"););

    ASSERT_NO_THROW(timer_manager.tic("sleep1"););

    std::this_thread::sleep_for(lambt::milliseconds(2));

    ASSERT_NO_THROW(timer_manager.toc("sleep1"););
  }

  ASSERT_NO_THROW(timer_manager.tic("sleep2"););

  std::this_thread::sleep_for(lambt::milliseconds(1));

  ASSERT_NO_THROW(timer_manager.toc("sleep2"););

  const auto reports = timer_manager.reports();

  ASSERT_TRUE(reports.size() == 3);

  std::string formated_report;
  ASSERT_NO_THROW(formated_report = lambt::ReportFormatter::format(reports););

  std::cout << lambt::ReportFormatter::format(timer_manager.report("sleep0")) << "\n" << std::endl;
  std::cout << lambt::ReportFormatter::format(timer_manager.report("sleep1")) << "\n" << std::endl;
  std::cout << lambt::ReportFormatter::format(timer_manager.report("sleep2")) << "\n" << std::endl;
  std::cout << formated_report << std::endl;
}


int main (int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

