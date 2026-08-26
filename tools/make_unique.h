#ifndef MAKE_UNIQUE_H
#define MAKE_UNIQUE_H

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

/*
 * This is the source code of make_unique as proposed in paper N3656 by Stephan T. Lavave
 *
 * std::make_unique is part of the standard library since C++14, so the backfill
 * below must only be compiled for C++11. Defining it anyway makes every
 * make_unique call ambiguous. MSVC reports the active standard in _MSVC_LANG
 * rather than __cplusplus.
 */
#if defined(_MSVC_LANG)
#define MAKE_UNIQUE_CPLUSPLUS _MSVC_LANG
#else
#define MAKE_UNIQUE_CPLUSPLUS __cplusplus
#endif

#if MAKE_UNIQUE_CPLUSPLUS < 201402L
namespace std {
  template<class T>
  struct _Unique_if {
    typedef unique_ptr<T> _Single_object;
  };

  template<class T>
  struct _Unique_if<T[]> {
    typedef unique_ptr<T[]> _Unknown_bound;
  };

  template<class T, size_t N>
  struct _Unique_if<T[N]> {
    typedef void _Known_bound;
  };

  template<class T, class... Args>
  typename _Unique_if<T>::_Single_object
  make_unique(Args &&...args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template<class T>
  typename _Unique_if<T>::_Unknown_bound
  make_unique(size_t n) {
    typedef typename remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]());
  }

  template<class T, class... Args>
  typename _Unique_if<T>::_Known_bound
  make_unique(Args &&...) = delete;
}// namespace std
#endif// MAKE_UNIQUE_CPLUSPLUS < 201402L
#endif// MAKE_UNIQUE_H
