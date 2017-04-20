#ifndef _LAMBT_MANAGER_HPP_
#define _LAMBT_MANAGER_HPP_

namespace lambt
{

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
template <typename... Keys>
TimerManager<Timer, Precision, Key>::TimerManager(Keys&&... keys) :
  timers_{{(typename Timers::value_type(keys, {}))...}}
{
  static_assert(detail::all_true<std::is_convertible<Keys, Key>::value...>::value,
                  "Error TimerManager::TimerManager :\nparameters must be of the type Key.");
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
void TimerManager<Timer, Precision, Key>::tic(const Key& key)
{
  timers_[key].tic();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision TimerManager<Timer, Precision, Key>::toc(const Key& key)
{
  return timers_[key].toc();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision TimerManager<Timer, Precision, Key>::last_elapsed(const Key& key)
{
  return timers_[key].last_elapsed();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision TimerManager<Timer, Precision, Key>::avg_toc(const Key& key)
{
  return timers_[key].avg_toc();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision TimerManager<Timer, Precision, Key>::variance_toc(const Key& key)
{
  return timers_[key].variance_toc();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
auto TimerManager<Timer, Precision, Key>::report(const Key& key) -> const report_t&
{
  return timers_[key].report();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
auto TimerManager<Timer, Precision, Key>::reports() -> reports_t
{
  reports_t reports;

  for (const auto& p : timers_)
    reports.emplace(p.first, p.second.report());

  return reports;
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
bool TimerManager<Timer, Precision, Key>::remove(const Key& key)
{
  return (bool)timers_.erase(key);
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
void TimerManager<Timer, Precision, Key>::clear()
{
  timers_.clear();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
std::vector<Key> TimerManager<Timer, Precision, Key>::keys() const
{
  std::vector<Key> keys;
  keys.reserve(timers_.size());

  for (const auto& p : timers_)
    keys.push_back(p.first);

  return keys;
}

} /* namespace lambt */

#endif /* _LAMBT_MANAGER_HPP_ */
