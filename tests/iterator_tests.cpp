/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_tests.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 12:55:53 by bcosters          #+#    #+#             */
/*   Updated: 2022/04/25 12:39:04 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IS_TEST
#include <algorithm>
// #include <bits/stdc++.h>
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
// #include "../include/Vector.hpp"
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
    iterator(long _num = 0) : num(_num) {}
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

public:
  typedef ft::iterator<ft::input_iterator_tag, int> It;
  MyIterator(pointer x) : ptr(x) {}
  MyIterator(const MyIterator &mit) : It(mit) {}
  MyIterator &operator++() {
    this->ptr++;
    return *this;
  }
  MyIterator operator++(int) {
    MyIterator tmp(*this);
    ++*this;
    return tmp;
  }
  bool operator==(const MyIterator &rhs) const { return this->ptr == rhs.ptr; }
  bool operator!=(const MyIterator &rhs) const { return this->ptr != rhs.ptr; }
  int &operator*() { return *this->ptr; }

private:
  pointer ptr;
};

class BidirectIt : public ft::iterator<ft::bidirectional_iterator_tag, int> {

public:
  typedef ft::iterator<ft::bidirectional_iterator_tag, int> It;
  typedef It::value_type value_type;
  typedef It::difference_type difference_type;
  typedef It::pointer pointer;
  typedef It::reference reference;
  typedef It::iterator_category iterator_category;

  BidirectIt(pointer x) : ptr(x) {}

  BidirectIt &operator++() {
    this->ptr++;
    return *this;
  }
  BidirectIt operator++(int) {
    BidirectIt tmp(*this);
    ++*this;
    return tmp;
  }
  bool operator==(BidirectIt const &rhs) { return (this->ptr == rhs.ptr); }
  bool operator!=(BidirectIt const &rhs) { return !(*this == rhs); }
  reference operator*() const { return *this->ptr; }
  pointer operator->() const { return this->ptr; }
  BidirectIt &operator--() {
    this->ptr--;
    return *this;
  };
  BidirectIt operator--(int) {
    BidirectIt tmp(*this);
    --*this;
    return tmp;
  };

private:
  pointer ptr;
};

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

//   int numbers[] = {10, 20, 30, 40, 50};
//   MyIterator from(numbers);
//   MyIterator until(numbers + 5);
//   for (MyIterator it = from; it != until; it++)
//     std::cout << *it << ' ';
//   std::cout << '\n';

  std::cout << "\nOUTPUT_ITERATOR\n";
  std::istringstream str("0.1 0.2 0.3 0.4");
  //   std::partial_sum(istream_iterator<double>(str),
  //   istream_iterator<double>(),
  //                    ostream_iterator<double>(std::cout, ","));
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