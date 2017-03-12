/**
 * \file timer.h
 *
 *  Created on: Feb 17, 2017
 *  \author: Jeremie Deray
 */

#ifndef _LAMBT_TIMER_REPORT_H_
#define _LAMBT_TIMER_REPORT_H_

namespace lambt
{

template <typename Unit>
struct TimerReport
{
  std::size_t calls = 0;
  Unit last_elapsed = Unit::zero();
  Unit average      = Unit::zero();
  Unit variance     = Unit::zero();
};

} /* namespace lambt */

#endif /* _LAMBT_TIMER_REPORT_H_ */
