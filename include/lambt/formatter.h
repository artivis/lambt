/**
 * \file timer.h
 *
 *  Created on: Feb 17, 2017
 *  \author: Jeremie Deray
 */

#ifndef _LAMBT_FORMATTER_H_
#define _LAMBT_FORMATTER_H_

#include <lambt/timer_report.h>

namespace lambt
{

enum class UnitFormat : std::size_t
{
  SHORT = 0,
  FULL
};

template <UnitFormat uf, typename Duration> struct to_str {};
template <> struct to_str<UnitFormat::SHORT, nanoseconds>  { static constexpr const char* c_str = "ns"; };
template <> struct to_str<UnitFormat::SHORT, microseconds> { static constexpr const char* c_str = "us"; };
template <> struct to_str<UnitFormat::SHORT, milliseconds> { static constexpr const char* c_str = "ms"; };
template <> struct to_str<UnitFormat::SHORT, seconds>      { static constexpr const char* c_str = "s";  };
template <> struct to_str<UnitFormat::SHORT, minutes>      { static constexpr const char* c_str = "mn"; };
template <> struct to_str<UnitFormat::SHORT, hours>        { static constexpr const char* c_str = "h";  };

template <> struct to_str<UnitFormat::FULL, nanoseconds>  { static constexpr const char* c_str = "nanoseconds";  };
template <> struct to_str<UnitFormat::FULL, microseconds> { static constexpr const char* c_str = "microseconds"; };
template <> struct to_str<UnitFormat::FULL, milliseconds> { static constexpr const char* c_str = "milliseconds"; };
template <> struct to_str<UnitFormat::FULL, seconds>      { static constexpr const char* c_str = "secondes";     };
template <> struct to_str<UnitFormat::FULL, minutes>      { static constexpr const char* c_str = "minutes";      };
template <> struct to_str<UnitFormat::FULL, hours>        { static constexpr const char* c_str = "hours";        };

struct ReportFormatter
{
  template<typename Unit>
  static std::string format(const TimerReport<Unit>& report)
  {
    std::stringstream string_report;

    /// @todo option SHORT/FULL
    constexpr auto unit_string = to_str<UnitFormat::SHORT, Unit>::c_str;

    const auto variance = report.variance.count();

    string_report
        << "\tlast:\t\t"   << report.last_elapsed.count() << unit_string << "\n"
        << "\taverage:\t"  << report.average.count()      << unit_string << "\n"
        << "\tstd:\t\t"    << std::sqrt(variance)         << unit_string << "\n"
        << "\tvariance:\t" << variance                    << unit_string << "\n"
        << "\tcalls:\t\t"  << report.calls;

    return string_report.str();
  }

  template<typename Key, typename Unit>
  static std::string format(const std::map<Key,TimerReport<Unit>>& reports)
  {
    std::stringstream string_report;

    for (const auto& p : reports)
    {
      string_report
          << "----------------------------------\n"
          << p.first          << ":\n"
          << format(p.second) << "\n";
    }

    return string_report.str();
  }
};

//struct Formatter
//{
//  template<typename Unit, typename _Rep, typename _Period>
//  static std::string format(const detail::duration<_Rep, _Period>& dur)
//  {
//    std::stringstream report;

//    report << duration_soft_cast<Unit>(dur) << " "
//           << to_str<UnitFormat::SHORT, Unit>::c_str;

//    return report.str();
//  }

//  template <typename Unit, typename Precision>
//  static std::string format(const Timer<Precision>& timer)
//  {
//    std::stringstream report;

//    report << "average : " << timer.avg_toc().count() << " "
//           << to_str<UnitFormat::SHORT, Unit>::c_str
//           << " over " << timer.calls() << " calls.";

//    return report.str();
//  }

//  template <typename Unit, typename Precision>
//  static std::string format(const AverageTimer<Precision>& timer)
//  {
//    std::stringstream report;

//    report << "average : " << format(timer.avg_toc()) << " "
//           << to_str<UnitFormat::SHORT, Unit>::c_str
//           << " over " << timer.calls() << " calls.";

//    return report.str();
//  }
//};

} /* namespace lambt */

#endif /* _LAMBT_FORMATTER_H_ */
