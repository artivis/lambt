#ifndef _LAMBT_TIMER_HPP_
#define _LAMBT_TIMER_HPP_

#include <algorithm>

namespace lambt
{

template <typename Precision>
Timer<Precision>::Timer() :
  last_start_()
{
  //std::cout << "Timer Timer." << std::endl;
}

template <typename Precision>
void Timer<Precision>::tic()
{
  last_start_ = detail::default_clock_t::now();
}

template <typename Precision>
auto Timer<Precision>::toc() -> Duration
{
  report_.last_elapsed = detail::duration_cast<Duration>(detail::default_clock_t::now() - last_start_);
  return report_.last_elapsed;
}

template <typename Precision>
auto Timer<Precision>::last_elapsed() const -> Duration
{
  return report_.last_elapsed;
}

template <typename Precision>
std::size_t Timer<Precision>::calls() const noexcept
{
  return 1;
}

template <typename Precision>
auto Timer<Precision>::report() const noexcept -> const report_t&
{
  do_report();
  return report_;
}

template <typename Precision>
void Timer<Precision>::do_report() const
{
  report_.calls   = calls();
  report_.average = report_.last_elapsed;
}

template <typename Precision>
AverageTimer<Precision>::AverageTimer(const std::size_t average_window_size):
  BaseTimer(), average_window_size_(average_window_size),
  elapsed_(average_window_size_, Precision::zero()) { }

template <typename Precision>
auto AverageTimer<Precision>::toc() -> Duration
{
  report_.last_elapsed = BaseTimer::toc();

  if (calls_ == average_window_size_) { calls_=0; filled = true; }

  elapsed_[calls_++] = report_.last_elapsed;

  avg_comp_ = false;
  var_comp_ = false;

  return report_.last_elapsed;
}

template <typename Precision>
auto AverageTimer<Precision>::avg_toc() const -> Duration
{
  if (calls_ == 0) return Duration(0);

  if (!avg_comp_)
  {
    report_.average = std::accumulate(elapsed_.begin(), elapsed_.end(),
                                      Duration::zero()) / calls();

    avg_comp_ = true;
  }

  return report_.average;
}

template <typename Precision>
auto AverageTimer<Precision>::variance_toc() const -> Duration
{
  if (calls_ == 0) return Duration(0);

  const auto ca = calls();

  std::vector<Duration> diff(ca);

  report_.average = avg_toc();

  std::transform(elapsed_.begin(), elapsed_.begin()+ca+1, diff.begin(),
                 [&](const Duration& t) {return t - report_.average;});

  if (!var_comp_)
  {
    report_.variance =
        std::inner_product(
            diff.begin(), diff.end(),
            diff.begin(), Duration::zero(),
            [](const Duration &a, const Duration &b) {return a+b;},
            [](const Duration &a, const Duration &b) {return Duration(a.count()*b.count());}
        ) / ca;

    var_comp_ = true;
  }

  return report_.variance;
}

template <typename Precision>
void AverageTimer<Precision>::reset()
{
  calls_    = 0;
  filled    = false;
  avg_comp_ = false;
  var_comp_ = false;
  report_   = typename BaseTimer::report_t();
  std::fill(elapsed_.begin(), elapsed_.end(), Duration::zero());
}

template <typename Precision>
std::size_t AverageTimer<Precision>::calls() const noexcept
{
  return filled ? average_window_size_ : calls_;
}

template <typename Precision>
void AverageTimer<Precision>::do_report() const
{
  variance_toc();
  report_.calls = calls();
}

} /* namespace lambt */

#endif /* _LAMBT_TIMER_HPP_ */
