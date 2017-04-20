#ifndef _LAMBT_LAMBT_H_
#define _LAMBT_LAMBT_H_

#include "lambt/timer_manager.h"
#include "lambt/formatter.h"
#include "lambt/singleton.h"

namespace lambt
{
namespace detail
{
using GlobalManager_t = TimerManager<AverageTimer, default_precision, std::string>;

struct GlobalManagerDeleter
{
  void operator()( /*const*/ GlobalManager_t* const p )
  {
    const auto reports = p->reports();
    delete p;
    std::cout << lambt::ReportFormatter::format(reports);
  }
};

} /* namespace detail */

using GlobalManager = Singleton< detail::GlobalManager_t, detail::GlobalManagerDeleter >;

//static TimerManager<AverageTimer, default_precision, std::string>::reports_t
//reports()
//{
//  return lambt::GlobalManager::get().reports();
//}

} /* namespace lambt */

#define STRINGIFY_IMPL(a) #a
#define STRINGIFY(a) STRINGIFY_IMPL(a)

#define CONCATENATE_IMPL(a,b) a##b
#define CONCATENATE(a,b) CONCATENATE_IMPL(a,b)

#ifdef __COUNTER__
#define ANONYMOUS_VAR(str)\
  CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VAR(str)\
  CONCATENATE(str, __LINE__)
#endif

#ifdef ENABLE_LAMBT

  #define LAMBT_TIC(Name)\
    lambt::GlobalManager::get().tic(Name);

  #define LAMBT_TOC(Name)\
    /*auto ANONYMOUS_VAR(toc) =*/ lambt::GlobalManager::get().toc(Name);

  #define LAMBT_TICTOC(func)\
    LAMBT_TIC(STRINGIFY(func)); \
    func; \
    LAMBT_TOC(STRINGIFY(func));

#else
  #define LAMBT_TIC(Name)
  #define LAMBT_TOC(Name)
  #define LAMBT_TICTOC(func)
#endif

#endif /* _LAMBT_LAMBT_H_ */
