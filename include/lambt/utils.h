/**
 * \file timer.h
 *
 *  Created on: Feb 17, 2017
 *  \author: Jeremie Deray
 */

#ifndef _LAMBT_UTILS_H_
#define _LAMBT_UTILS_H_

#include <lambt/internal/lambt_config.h>

#include <chrono>
#include <ctime>

namespace lambt
{

namespace detail
{
template<bool...> struct bool_pack;
template<bool... bs>
using all_true = std::is_same<bool_pack<bs..., true>,
                              bool_pack<true,  bs...>>;

template<typename _Clock, typename _Dur>
using time_point = std::chrono::time_point<_Clock, _Dur>;

template<typename _Rep, typename _Period>
using duration = std::chrono::duration<_Rep, _Period>;

using std::chrono::time_point_cast;
using std::chrono::duration_cast;

using default_clock_t = std::conditional< std::chrono::high_resolution_clock::is_steady,
                                          std::chrono::high_resolution_clock,
                                          std::chrono::steady_clock >::type;
} // namespace detail

using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

using default_precision = microseconds;

template<typename Unit, typename _Rep, typename _Period>
double duration_soft_cast(const detail::duration<_Rep, _Period>& dur)
{
  static constexpr auto ratio =
      static_cast<double>(std::ratio<typename Unit::period().den,
        typename detail::duration<_Rep, _Period>::period().den>().den);

  return static_cast<double>(dur.count()) / ratio;
}

} /* namespace lambt */

#endif /* _LAMBT_UTILS_H_ */
