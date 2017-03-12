/**
 * \file singleton.h
 *
 *  Created on: Aug 31, 2016
 *  \author: Jeremie Deray
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <memory>

/**
* \brief A thread-safer Singleton implementation with
* argument forwarding.
**/
template <class T, typename Deleter = std::default_delete<T>>
class Singleton
{
  using type          = Singleton<T, Deleter>;
  using hold_type     = T;
  using hold_type_ptr = std::unique_ptr<T, Deleter>;

public:

  template <typename... Args>
  static T& get(Args&&... args)
  {
    static hold_type_ptr instance_(new T(std::forward<Args>(args)...));

    return *instance_;
  }

  constexpr Singleton(const Singleton&) = delete;
  constexpr Singleton(Singleton&&)      = delete;

  constexpr Singleton& operator=(const Singleton&) = delete;
  constexpr Singleton& operator=(Singleton&&)      = delete;

private:

  Singleton()  = default;
  ~Singleton() = default;
};

#endif /* SINGLETON_H_ */
