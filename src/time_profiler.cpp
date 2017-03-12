#include "lambt/timer_manager.h"
#include "lambt/async_timer_manager.h"
#include "lambt/formatter.h"

#include <iostream>
#include <thread>

int main(int /*argc*/, char **/*argv*/)
{
//  lambt::TimerN nano_timer;

//  nano_timer.tic();

//  std::this_thread::sleep_for(lambt::milliseconds(1));

//  auto toc = nano_timer.toc();

//  std::cout << toc.count() << std::endl;


  lambt::NamedTimerManagerU timer_manager("toto", "tata");

  for (std::size_t i = 0; i<5; ++i)
  {
    timer_manager.tic("toto");

    std::this_thread::sleep_for(lambt::milliseconds(1));

    std::cout << " " << timer_manager.toc("toto").count();
  }

  auto toc1 = timer_manager.toc("toto");

  std::cout << "\n toc :\n" << toc1.count();

  std::cout << "\n soft toc :\n"
            << lambt::Formatter::format<lambt::seconds>(toc1) << std::endl;

  LAMBT_ELAPSED_NAMED("key", std::this_thread::sleep_for(lambt::milliseconds(1)));

  std::cout << "\n toc :\n" << lambt::get_elapsed("key").count();

  return EXIT_SUCCESS;
}
