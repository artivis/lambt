#ifndef _LAMBT_ASYNC_MANAGER_HPP_
#define _LAMBT_ASYNC_MANAGER_HPP_

namespace lambt
{

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
void AsyncTimerManager<Timer, Precision, Key>::tic(const Key& key)
{
  std::lock_guard<std::mutex> lock(access_mut_);
  timers_[key].tic();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision AsyncTimerManager<Timer, Precision, Key>::toc(const Key& key)
{
  std::lock_guard<std::mutex> lock(access_mut_);
  return timers_[key].toc();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision AsyncTimerManager<Timer, Precision, Key>::last_elapsed(const Key& key)
{
  std::lock_guard<std::mutex> lock(access_mut_);
  return timers_[key].last_elapsed();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision AsyncTimerManager<Timer, Precision, Key>::avg_toc(const Key& key)
{
  std::lock_guard<std::mutex> lock(access_mut_);
  return timers_[key].avg_toc();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
Precision AsyncTimerManager<Timer, Precision, Key>::variance_toc(const Key& key)
{
  std::lock_guard<std::mutex> lock(access_mut_);
  return timers_[key].variance_toc();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
auto AsyncTimerManager<Timer, Precision, Key>::report(const Key& key) -> const report_t&
{
  std::lock_guard<std::mutex> lock(access_mut_);
  return timers_[key].report();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
auto AsyncTimerManager<Timer, Precision, Key>::reports() -> reports_t
{
  reports_t reports;

  {
    std::lock_guard<std::mutex> lock(access_mut_);

    for (const auto& p : timers_)
      reports.emplace(p.first, p.second.report());
  }

  return reports;
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
bool AsyncTimerManager<Timer, Precision, Key>::remove(const Key& key)
{
  std::lock_guard<std::mutex> lock(access_mut_);
  return (bool)timers_.erase(key);
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
void AsyncTimerManager<Timer, Precision, Key>::clear()
{
  std::lock_guard<std::mutex> lock(access_mut_);
  timers_.clear();
}

template <template <typename Precision> class Timer,
          typename Precision, typename Key>
std::vector<Key> AsyncTimerManager<Timer, Precision, Key>::keys() const
{
  std::vector<Key> keys;
  keys.reserve(timers_.size());

  {
    std::lock_guard<std::mutex> lock(access_mut_);

    for (const auto& p : timers_)
      keys.push_back(p.first);
  }

  return keys;
}

} /* namespace lambt */

#endif /* _LAMBT_ASYNC_MANAGER_HPP_ */
