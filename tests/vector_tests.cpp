/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_tests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:53:16 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/20 17:31:06 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <sys/types.h>
#ifndef _IS_TEST
#include <algorithm>
// #include <bits/stdc++.h>
#include <iterator>
#include <map>
#include <memory>
#include <stack>
#include <type_traits>
#include <vector>
namespace ft = std;
#else
#include "../include/Iterators.hpp"
#include "../include/Map.hpp"
#include "../include/Stack.hpp"
#include "../include/Vector.hpp"
#include "../include/utility.hpp"
#endif // _IS_TEST
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <limits>

std::ostream &operator<<(std::ostream &s, const ft::vector<std::string> &v) {
  s.put('[');
  char comma[3] = {'\0', ' ', '\0'};
  for (ft::vector<std::basic_string<char> >::const_iterator it = v.begin();
       it != v.end(); it++) {
    s << comma << *it;
    comma[0] = ',';
  }
  return s << ']';
}

std::ostream &operator<<(std::ostream &s, const ft::vector<int> &v) {
  s.put('[');
  char comma[3] = {'\0', ' ', '\0'};
  for (ft::vector<int>::const_iterator it = v.begin(); it != v.end(); it++) {
    s << comma << *it;
    comma[0] = ',';
  }
  return s << ']';
}

template <class T> void print_vector(const ft::vector<T> &v) {
  typename ft::vector<T>::const_iterator it = v.begin();
  while (it != v.end()) {
    std::cout << *it++ << "\n";
  }
}

template <class Tp> struct NAlloc {
  typedef Tp value_type;
  typedef Tp *pointer;
  typedef Tp const *const_pointer;
  typedef Tp &reference;
  typedef Tp const &const_reference;
  typedef std::size_t size_type;
  typedef std::size_t difference_type;
  NAlloc(){};
  template <class T> NAlloc(const NAlloc<T> &) {}

  Tp *allocate(std::size_t n) {
    n *= sizeof(Tp);
    Tp *p = static_cast<Tp *>(::operator new(n));
    std::cout << "allocating " << n << " bytes @ "<< '\n';
    return p;
  }

  void deallocate(Tp *p, std::size_t n) {
    std::cout << "deallocating " << n * sizeof *p << " bytes @ "<< "\n\n";
    ::operator delete(p);
  }
  void destroy(Tp * p) { p->~Tp(); }
  void construct(Tp * p, value_type val) { *p = Tp(val); }
  size_type max_size() const {
    return (std::numeric_limits<size_type>::max()) / sizeof(value_type);
  }
};
template <class T, class U>
bool operator==(const NAlloc<T> &, const NAlloc<U> &) {
  return true;
}
template <class T, class U>
bool operator!=(const NAlloc<T> &, const NAlloc<U> &) {
  return false;
}

void vector_tests() {

  std::cout << "\nBASIC\n";
  // Create a vector containing integers
  ft::vector<int> v;

  // Add two more integers to vector
  std::cout << "push_back\n";
  v.push_back(25);
  v.push_back(13);
  std::cout << "push_back\n";

  // Print out the vector
  std::cout << "v = { ";
  for (ft::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
    std::cout << *it << ", ";
  }
  std::cout << "}; \n";

  std::cout << "\nCTOR\n";

  ft::vector<std::string> words1;
  words1.push_back("the");
  words1.push_back("frogurt");
  words1.push_back("is");
  words1.push_back("also");
  words1.push_back("cursed");

  std::cout << "words1: " << words1 << '\n';

  // words2 == words1
  ft::vector<std::string> words2(words1.begin(), words1.end());
  std::cout << "words2: " << words2 << '\n';

  // words3 == words1
  ft::vector<std::string> words3(words1);
  std::cout << "words3: " << words3 << '\n';

  // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
  ft::vector<std::string> words4(5, "Mo");
  std::cout << "words4: " << words4 << '\n';

  std::cout << "\nOPERATOR=\n";
  // OPERATOR=
  ft::vector<int> x, y;
  x.push_back(1);
  x.push_back(2);
  x.push_back(3);

  std::cout << "Initially:\n";
  std::cout << "x = " << x << "\n";
  std::cout << "y = " << y << "\n";

  std::cout << "Copy assignment copies data from x to y:\n";
  y = x;
  std::cout << "x = " << x << "\n";
  std::cout << "y = " << y << "\n";

  std::cout << "\nASSIGN\n";
  // ASSIGN
  ft::vector<char> characters;

  characters.assign(5, 'a');
  print_vector(characters);

  const std::string extra(6, 'b');
  characters.assign(extra.begin(), extra.end());
  print_vector(characters);

  std::cout << "\nAT + OPERATOR[]\n";
  // AT + []
  ft::vector<int> data;
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);

  // Set element 1 and 3
  data.at(1) = 88;
  data[3] = 69;

  // Read element 2 and 4
  std::cout << "Element at index 2 has value " << data.at(2) << '\n';
  std::cout << "Element at index 4 has value " << data[4] << '\n';

  std::cout << "data size = " << data.size() << '\n';

  try {
    // Set element 6
    data.at(6) = 666;
  } catch (std::out_of_range const &exc) {
    std::cout << exc.what() << '\n';
  }
  // Accessing an element out of range with [] is undefined behaviour

  // Print final values
  std::cout << "data:" << data;
  std::cout << '\n';

  std::cout << "\nFRONT + BACK\n";

  std::cout << "First element is " << words1.front() << '\n';
  std::cout << "First element is " << x.front() << '\n';
  std::cout << "First element is " << v.front() << '\n';
  std::cout << "First element is " << data.front() << '\n';
  std::cout << "Last element is " << words1.back() << '\n';
  std::cout << "Last element is " << x.back() << '\n';
  std::cout << "Last element is " << v.back() << '\n';
  std::cout << "Last element is " << data.back() << '\n';

  std::cout << "\nITERATOR + REVERSE_ITERATOR\n";

  ft::vector<std::string>::iterator it = words1.begin();
  ft::vector<std::string>::iterator ite = words1.end();
  for (; it != ite; it++) {
    std::cout << *it << '\n';
  }
  ft::vector<std::string>::const_iterator cit = words1.begin();
  ft::vector<std::string>::const_iterator cite = words1.end();
  for (; cit != cite; cit++) {
    std::cout << *cit << '\n';
  }
  ft::vector<std::string>::reverse_iterator rit = words1.rbegin();
  ft::vector<std::string>::reverse_iterator rite = words1.rend();
  for (; rit != rite; rit++) {
    std::cout << *rit << '\n';
  }
  ft::vector<std::string>::const_reverse_iterator rcit = words1.rbegin();
  ft::vector<std::string>::const_reverse_iterator rcite = words1.rend();
  for (; rcit != rcite; rcit++) {
    std::cout << *rcit << '\n';
  }

  std::cout << "\nEMPTY\n";

  std::cout << std::boolalpha;
  ft::vector<int> numbers;
  std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';

  numbers.push_back(42);
  std::cout << "After adding elements, numbers.empty(): " << numbers.empty()
            << '\n';

  std::cout << "\nSIZE + MAX_SIZE\n";

  std::cout << "nums contains " << numbers.size() << " elements.\n";
  std::cout << "nums has max_size = " << numbers.max_size() << " elements.\n";

  std::cout << "\nRESERVE\n";

  const int max_elements = 32;

  std::cout << "using reserve: \n";
  {
    ft::vector<int, NAlloc<int> > v1;
    v1.reserve(
        max_elements); // reserves at least max_elements * sizeof(int) bytes

    for (int n = 0; n < max_elements; ++n)
      v1.push_back(n);
  }

  std::cout << "not using reserve: \n";
  {
    ft::vector<int, NAlloc<int> > v1;

    for (int n = 0; n < max_elements; ++n) {
      if (v1.size() == v1.capacity()) {
        std::cout << "size() == capacity() == " << v1.size() << '\n';
      }
      v1.push_back(n);
    }
  }

  std::cout << "\nBEGIN/END\n";

  ft::vector<int> myvector;
  for (int i=1; i<=5; i++) myvector.push_back(i);

  std::cout << "myvector contains:";
  for (ft::vector<int>::iterator it = myvector.begin() ; it != myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nRBEGIN/REND\n";

  int i=0;

  ft::vector<int>::reverse_iterator rIt = myvector.rbegin();
  for (; rIt!= myvector.rend(); ++rIt)
    *rIt = ++i;

  std::cout << "myvector contains:";
  for (ft::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nRESIZE\n";

  myvector.resize(5);
  std::cout << "resize\n";
  myvector.resize(8,100);
  std::cout << "resize\n";
  myvector.resize(12);
  std::cout << "resize\n";

  std::cout << "myvector contains:";
  for (ft::vector<int>::size_type i=0;i<myvector.size();i++)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  std::cout << "\nSIZE/CAPACITY/MAX_SIZE\n";

  for (int i=0; i<100; i++) myvector.push_back(i);

  std::cout << "size: " << (int) myvector.size() << '\n';
  std::cout << "capacity: " << (int) myvector.capacity() << '\n';
  std::cout << "max_size: " << (int) myvector.max_size() << '\n';

  std::cout << "\nPOP_BACK\n";

  int sum (0);
  myvector.push_back (100);
  myvector.push_back (200);
  myvector.push_back (300);

  while (!myvector.empty())
  {
    sum+=myvector.back();
    myvector.pop_back();
  }

  std::cout << "The elements of myvector add up to " << sum << '\n';

  std::cout << "\nINSERT\n";

  ft::vector<int>::iterator iT;

  iT = myvector.begin();
  iT = myvector.insert ( iT , 200 );

  myvector.insert (iT,2,300);

  // "it" no longer valid, get a new one:
  iT = myvector.begin();

  ft::vector<int> anothervector (2,400);
  myvector.insert (iT+2,anothervector.begin(),anothervector.end());

  int myarray [] = { 501,502,503 };
  myvector.insert (myvector.begin(), myarray, myarray+3);

  std::cout << "myvector contains:";
  for (iT=myvector.begin(); iT<myvector.end(); iT++)
    std::cout << ' ' << *iT;
  std::cout << '\n';

  std::cout << "\nERASE\n";

 // set some values (from 1 to 10)
  for (int i=1; i<=10; i++) myvector.push_back(i);

  // erase the 6th element
  myvector.erase (myvector.begin()+5);

  // erase the first 3 elements:
  myvector.erase (myvector.begin(),myvector.begin()+3);

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); ++i)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  std::cout << "\nSWAP\n";

  ft::vector<int> foo (3,100);   // three ints with a value of 100
  ft::vector<int> bar (5,200);   // five ints with a value of 200

  foo.swap(bar);

  std::cout << "foo contains:";
  for (unsigned i=0; i<foo.size(); i++)
    std::cout << ' ' << foo[i];
  std::cout << '\n';

  std::cout << "bar contains:";
  for (unsigned i=0; i<bar.size(); i++)
    std::cout << ' ' << bar[i];
  std::cout << '\n';

  std::cout << "\nCLEAR\n";

  myvector.push_back (100);
  myvector.push_back (200);
  myvector.push_back (300);

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); i++)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  myvector.clear();
  myvector.push_back (1101);
  myvector.push_back (2202);

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); i++)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  std::cout << "\nGET_ALLOCATOR\n";

  int * p;
  unsigned int i2;

  // allocate an array with space for 5 elements using vector's allocator:
  p = myvector.get_allocator().allocate(5);

  // construct values in-place on the array:
  for (i2=0; i2<5; i2++) myvector.get_allocator().construct(&p[i],i);

  std::cout << "The allocated array contains:";
  for (i2=0; i2<5; i2++) std::cout << ' ' << p[i2];
  std::cout << '\n';

  // destroy and deallocate:
  for (i2=0; i2<5; i2++) myvector.get_allocator().destroy(&p[i2]);
  myvector.get_allocator().deallocate(p,5);

  std::cout << "\nCOMPARISON_OPERATORS\n";

  if (foo==bar) std::cout << "foo and bar are equal\n";
  if (foo!=bar) std::cout << "foo and bar are not equal\n";
  if (foo< bar) std::cout << "foo is less than bar\n";
  if (foo> bar) std::cout << "foo is greater than bar\n";
  if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
  if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
}

int main() {
  vector_tests();
  return 0;
}