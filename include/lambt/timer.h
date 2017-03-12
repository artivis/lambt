/**
 * \file timer.h
 *
 *  Created on: Feb 17, 2017
 *  \author: Jeremie Deray
 */

#ifndef _LAMBT_TIMER_H_
#define _LAMBT_TIMER_H_

#include <lambt/utils.h>
#include <lambt/timer_report.h>

#include <utility>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>

#include <iostream>

namespace lambt
{

/**
 * @brief Timer. A tic timer.
 *
 * Mesure the elapsed time between construction - or tic() -
 * to tic(). The elapsed time is expressed in unit.
 *
 * @param unit. The time unit.
 * @see unit
 */
template <typename Precision>
class Timer
{
protected:

  using default_clock_t = detail::default_clock_t;
  using time_point_t    = default_clock_t::time_point;

public:

  using Duration = Precision;
  using report_t = TimerReport<Precision>;

  /**
   * @brief Timer.
   */
  Timer();

  /**
   * @brief ~Timer. Default desctructor.
   */
  virtual ~Timer() = default;

  /**
   * @brief toc. Return elapsed time since construction or last tic().
   * @return T. The elapsed time in unit.
   */
  virtual void tic();

  virtual Duration toc();

  virtual Duration last_elapsed() const;

  /**
    * @brief getPtr
    */
  //   void ptr();

  virtual std::size_t calls() const noexcept;

  const report_t& report() const noexcept;

protected:

  time_point_t last_start_;

  mutable report_t report_;

  virtual void do_report() const;
};

using TimerS = Timer<seconds>;
using TimerM = Timer<milliseconds>;
using TimerU = Timer<microseconds>;
using TimerN = Timer<nanoseconds>;

using DefaultTimer = Timer<default_precision>;

template <typename Precision>
class AverageTimer : public Timer<Precision>
{
protected:

  using BaseTimer = Timer<Precision>;
  using typename BaseTimer::Timer;
  using typename BaseTimer::time_point_t;
  using BaseTimer::report_;
  using BaseTimer::last_start_;

public:

  using typename Timer<Precision>::Duration;

  /**
   * @brief Timer.
   */
  AverageTimer(const std::size_t average_window_size = 10);

  /**
   * @brief ~Timer. Default desctructor.
   */
  virtual ~AverageTimer() = default;

   /**
    * @brief toc. Return elapsed time since construction or last tic().
    * @return T. The elapsed time in unit.
    */
   virtual Duration toc() override;

   /**
    * @brief avg_toc. Return the average toc.
    * @return T. The average elapsed time in unit
    * @see tic()
    * @see toc()
    * @see unit
    */
   Duration avg_toc() const;

   Duration variance_toc() const;

   /**
    * @brief reset. Reset the timer average.
    */
   inline void reset();

   /**
    * @brief calls. The number of times tic() was called.
    * @return std::size_t. The number of times tic() was called.
    */
   inline std::size_t calls() const noexcept override;

   const std::size_t average_window_size_;

protected:

   bool filled = false;

   std::size_t calls_ = 0;

   std::vector<Duration> elapsed_;

   virtual void do_report() const override;
};

using AverageTimerS = AverageTimer<seconds>;
using AverageTimerM = AverageTimer<milliseconds>;
using AverageTimerU = AverageTimer<microseconds>;
using AverageTimerN = AverageTimer<nanoseconds>;

using DefaultAverageTimer = AverageTimer<default_precision>;

template <typename Precision>
class TimerCallable : protected AverageTimer<Precision>
{
protected:

  using BaseTimer = AverageTimer<Precision>;

  using BaseTimer::reset;
  using BaseTimer::tic;
  using BaseTimer::toc;
  using BaseTimer::avg_toc;
  using BaseTimer::calls_;

public:

  TimerCallable()  = default;
  ~TimerCallable() = default;

  template <typename Callable>
  TimerReport<Precision> elapsed(Callable&& f)
  {
    //reset();

    tic();
    f();
    const auto toc = toc();

    return TimerReport<Precision>(toc, calls_);
  }

  template <typename Callable>
  TimerReport<Precision> elapsed(Callable&& f, const std::size_t iterations)
  {
    reset();

    for (std::size_t i=0; i<iterations; ++i)
    {
      elapsed(std::forward<Callable>(f));
    }

    return TimerReport<Precision>(avg_toc(), calls_);
  }

  template <typename Callable, typename CallDur, typename CallPer>
  TimerReport<Precision> elapsed(Callable&& f,
                      const detail::duration<CallDur, CallPer> duration_call =
                       seconds(1))
  {
    reset();

    AverageTimer<Precision> ext_timer;
    ext_timer.tic();

    typename AverageTimer<Precision>::Duration total_elapsed;

    do
    {
      elapsed(std::forward<Callable>(f));
      total_elapsed = ext_timer.toc();
    } while (total_elapsed + ext_timer.avg_toc() < duration_call);

    return TimerReport<Precision>(avg_toc(), calls_);
  }
};

} /* namespace lambt */

#include "lambt/impl/timer.hpp"

#endif /* _LAMBT_TIMER_H_ */
