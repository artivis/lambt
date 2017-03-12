#ifndef _LAMBT_TIMER_MANAGER_H_
#define _LAMBT_TIMER_MANAGER_H_

//#include <unordered_map>
#include <map>

#include "lambt/timer.h"

namespace lambt
{

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
class TimerManager
{
public:

  using key_t       = Key;
  using precision_t = Precision;
  using timer_t     = Timer<Precision>;

  using report_t    = typename timer_t::report_t;
  using reports_t   = std::map<key_t, report_t>;

protected:

  //using Timers = std::unordered_map<key_t, timer_t>;
  using Timers = std::map<key_t, timer_t>;

  // Not copyable
  TimerManager(const TimerManager&) = delete;
  TimerManager& operator=(const TimerManager&) = delete;

public:

  TimerManager()  = default;
  ~TimerManager() = default;

  template <typename... Keys>
  TimerManager(Keys&&... keys);

  void tic(Key&& key);

  Precision toc(Key&& key);

  Precision last_elapsed(Key&& key);

  Precision avg_toc(Key&& key);

  Precision variance_toc(Key&& key);

  const report_t& report(Key&& key);

  reports_t reports();

  std::vector<Key> keys() const;

//  void reset(Key&& key);

//  std::size_t calls(Key&& key);

  bool remove(Key&& key);

  void clear();

protected:

  Timers timers_;
};

} /* namespace lambt */

#include "lambt/impl/timer_manager.hpp"

#endif /* _LAMBT_TIMER_MANAGER_H_ */
