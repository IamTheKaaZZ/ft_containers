/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_tests.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 12:55:53 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/23 10:43:05 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IS_TEST
#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <map>
#include <memory>
#include <stack>
#include <type_traits>
namespace ft = std;
#else
#include "../include/Iterators.hpp"
#include "../include/Map.hpp"
#include "../include/Stack.hpp"
#include "../include/Vector.hpp"
#include "../include/utility.hpp"
#endif // _IS_TEST
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdlib.h>
#include <vector>

template <long FROM, long TO> class Range {
public:
  // member typedefs provided through inheriting from std::iterator
  class iterator
      : public ft::iterator<ft::input_iterator_tag, // iterator_category
                            long,                   // value_type
                            long,                   // difference_type
                            const long *,           // pointer
                            long                    // reference
                            > {
    long num;

  public:
    explicit iterator(long _num = 0) : num(_num) {}
    iterator &operator++() {
      num = TO >= FROM ? num + 1 : num - 1;
      return *this;
    }
    iterator operator++(int) {
      iterator retval = *this;
      ++(*this);
      return retval;
    }
    bool operator==(iterator other) const { return num == other.num; }
    bool operator!=(iterator other) const { return !(*this == other); }
    reference operator*() const { return num; }
  };
  iterator begin() { return iterator(FROM); }
  iterator end() { return iterator(TO >= FROM ? TO + 1 : TO - 1); }
};

class MyIterator : public ft::iterator<ft::input_iterator_tag, int> {
  int *p;

public:
  MyIterator(int *x) : p(x) {}
  MyIterator(const MyIterator &mit) : p(mit.p) {}
  MyIterator &operator++() {
    ++p;
    return *this;
  }
  MyIterator operator++(int) {
    MyIterator tmp(*this);
    operator++();
    return tmp;
  }
  bool operator==(const MyIterator &rhs) const { return p == rhs.p; }
  bool operator!=(const MyIterator &rhs) const { return p != rhs.p; }
  int &operator*() { return *p; }
};

class BidirectIt : public ft::iterator<ft::bidirectional_iterator_tag, int> {
  int *p;

public:
  typedef ft::iterator<ft::bidirectional_iterator_tag, int> It;
  typedef It::value_type value_type;
  typedef It::difference_type difference_type;
  typedef It::pointer pointer;
  typedef It::reference reference;
  typedef It::iterator_category iterator_category;

  BidirectIt() : p(NULL) {}
  BidirectIt(int *x) : p(x) {}

  BidirectIt &operator++() {
    ++p;
    return *this;
  }
  BidirectIt operator++(int) {
    BidirectIt tmp(*this);
    ++*this;
    return tmp;
  }
  bool operator==(BidirectIt const &rhs) { return (this->p == rhs.p); }
  bool operator!=(BidirectIt const &rhs) { return !(*this == rhs); }
  reference operator*() const { return *p; }
  pointer operator->() const { return p; }
  BidirectIt &operator--() {
    this->p--;
    return *this;
  };
  BidirectIt operator--(int) {
    BidirectIt tmp(*this);
    --*this;
    return tmp;
  };
};

/**
 *  @brief  Provides output iterator semantics for streams.
 *
 *  This class provides an iterator to write to an ostream.  The type Tp is
 *  the only type written by this iterator and there must be an
 *  operator<<(Tp) defined.
 *
 *  @tparam  _Tp  The type to write to the ostream.
 *  @tparam  _CharT  The ostream char_type.
 *  @tparam  _Traits  The ostream char_traits.
 */
template <typename _Tp, typename _CharT = char,
          typename _Traits = std::char_traits<_CharT> >
class ostream_iterator
    : public ft::iterator<ft::output_iterator_tag, void, void, void, void> {
public:
  //@{
  /// Public typedef
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef std::basic_ostream<_CharT, _Traits> ostream_type;
  //@}
private:
  ostream_type *_M_stream;
  const _CharT *_M_string;

public:
  /// Construct from an ostream.
  ostream_iterator(ostream_type &__s)
      : _M_stream(std::__addressof(__s)), _M_string(0) {}
  /**
   *  Construct from an ostream.
   *
   *  The delimiter string @a c is written to the stream after every Tp
   *  written to the stream.  The delimiter is not copied, and thus must
   *  not be destroyed while this iterator is in use.
   *
   *  @param  __s  Underlying ostream to write to.
   *  @param  __c  CharT delimiter string to insert.
   */
  ostream_iterator(ostream_type &__s, const _CharT *__c)
      : _M_stream(&__s), _M_string(__c) {}
  /// Copy constructor.
  ostream_iterator(const ostream_iterator &__obj)
      : _M_stream(__obj._M_stream), _M_string(__obj._M_string) {}
  /// Writes @a value to underlying ostream using operator<<.  If
  /// constructed with delimiter string, writes delimiter to ostream.
  ostream_iterator &operator=(const _Tp &__value) {
    __glibcxx_requires_cond(
        _M_stream != 0,
        _M_message(__gnu_debug::__msg_output_ostream)._M_iterator(*this));
    *_M_stream << __value;
    if (_M_string)
      *_M_stream << _M_string;
    return *this;
  }
  ostream_iterator &operator*() { return *this; }
  ostream_iterator &operator++() { return *this; }
  ostream_iterator &operator++(int) { return *this; }
};

template <typename _Tp, typename _CharT = char,
          typename _Traits = std::char_traits<_CharT>,
          typename _Dist = ptrdiff_t>
class istream_iterator : public ft::iterator<ft::input_iterator_tag, _Tp, _Dist,
                                             const _Tp *, const _Tp &> {
public:
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef std::basic_istream<_CharT, _Traits> istream_type;

private:
  istream_type *_M_stream;
  _Tp _M_value;
  bool _M_ok;

public:
  ///  Construct end of input stream iterator.
  _GLIBCXX_CONSTEXPR istream_iterator()
      : _M_stream(0), _M_value(), _M_ok(false) {}
  ///  Construct start of input stream iterator.
  istream_iterator(istream_type &__s) : _M_stream(std::__addressof(__s)) {
    _M_read();
  }
  istream_iterator(const istream_iterator &__obj)
      : _M_stream(__obj._M_stream), _M_value(__obj._M_value),
        _M_ok(__obj._M_ok) {}
  const _Tp &operator*() const {
    __glibcxx_requires_cond(
        _M_ok, _M_message(__gnu_debug::__msg_deref_istream)._M_iterator(*this));
    return _M_value;
  }
  const _Tp *operator->() const { return std::__addressof((operator*())); }
  istream_iterator &operator++() {
    __glibcxx_requires_cond(
        _M_ok, _M_message(__gnu_debug::__msg_inc_istream)._M_iterator(*this));
    _M_read();
    return *this;
  }
  istream_iterator operator++(int) {
    __glibcxx_requires_cond(
        _M_ok, _M_message(__gnu_debug::__msg_inc_istream)._M_iterator(*this));
    istream_iterator __tmp = *this;
    _M_read();
    return __tmp;
  }
  bool _M_equal(const istream_iterator &__x) const {
    return (_M_ok == __x._M_ok) && (!_M_ok || _M_stream == __x._M_stream);
  }

private:
  void _M_read() {
    _M_ok = (_M_stream && *_M_stream) ? true : false;
    if (_M_ok) {
      *_M_stream >> _M_value;
      _M_ok = *_M_stream ? true : false;
    }
  }
};

///  Return true if x and y are both end or not end, or x and y are the same.
template <typename _Tp, typename _CharT, typename _Traits, typename _Dist>
inline bool
operator==(const istream_iterator<_Tp, _CharT, _Traits, _Dist> &__x,
           const istream_iterator<_Tp, _CharT, _Traits, _Dist> &__y) {
  return __x._M_equal(__y);
}
///  Return false if x and y are both end or not end, or x and y are the same.
template <class _Tp, class _CharT, class _Traits, class _Dist>
inline bool
operator!=(const istream_iterator<_Tp, _CharT, _Traits, _Dist> &__x,
           const istream_iterator<_Tp, _CharT, _Traits, _Dist> &__y) {
  return !__x._M_equal(__y);
}

template <typename T, size_t SIZE> class Stack {
  T arr[SIZE];
  size_t pos;

public:
  Stack() : pos(0) {}
  T pop() { return arr[--pos]; }
  Stack &push(const T &t) {
    arr[pos++] = t;
    return *this;
  }
  // we wish that looping on Stack would be in LIFO order
  // thus we use std::reverse_iterator as an adaptor to existing iterators
  // (which are in this case the simple pointers: [arr, arr+pos)
  ft::reverse_iterator<BidirectIt> begin() {
    return ft::reverse_iterator<BidirectIt>(arr + pos);
  }
  ft::reverse_iterator<BidirectIt> end() {
    return ft::reverse_iterator<BidirectIt>(arr);
  }
};

void tests() {
  std::cout << "\nINPUT_ITERATOR\n";
  Range<15, 25> range;

  for (Range<15, 25>::iterator it = range.begin(); it != range.end(); it++) {
    std::cout << *it << ' '; // print entire range
  }
  std::cout << '\n';

  int numbers[] = {10, 20, 30, 40, 50};
  MyIterator from(numbers);
  MyIterator until(numbers + 5);
  for (MyIterator it = from; it != until; it++)
    std::cout << *it << ' ';
  std::cout << '\n';

  std::cout << "\nOUTPUT_ITERATOR\n";
  std::istringstream str("0.1 0.2 0.3 0.4");
  std::partial_sum(istream_iterator<double>(str), istream_iterator<double>(),
                   ostream_iterator<double>(std::cout, ","));
  std::cout << "\n";

  std::cout << "\nREVERSE_ITERATOR\n";
  Stack<int, 8> s;
  s.push(5).push(15).push(25).push(35);
  for (ft::reverse_iterator<BidirectIt> it = s.begin(); it != s.end(); it++) {
    std::cout << *it << ' ';
  }
}

int main() {
  tests();
  return 0;
}