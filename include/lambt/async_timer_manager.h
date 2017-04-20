#ifndef _LAMBT_ASYNC_TIMER_MANAGER_H_
#define _LAMBT_ASYNC_TIMER_MANAGER_H_

#include <mutex>

#include "lambt/timer_manager.h"

namespace lambt
{

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
class AsyncTimerManager : public TimerManager<Timer, Precision, Key>
{
public:

  using Base = TimerManager<Timer, Precision, Key>;
  using Base::Base;

protected:

  using Base::timers_;

public:

  using typename Base::report_t;
  using typename Base::reports_t;

  ~AsyncTimerManager() = default;

  void tic(const Key& key);

  Precision toc(const Key& key);

  Precision last_elapsed(const Key& key);

  Precision avg_toc(const Key& key);

  Precision variance_toc(const Key& key);

  const report_t& report(const Key& key);

  reports_t reports();

  std::vector<Key> keys() const;

//  void reset(Key&& key);

//  std::size_t calls(Key&& key);

  bool remove(const Key& key);

  void clear();

protected:

  std::mutex access_mut_;
};

} /* namespace lambt */

#include "lambt/impl/async_timer_manager.hpp"

#endif /* _LAMBT_ASYNC_TIMER_MANAGER_H_ */
