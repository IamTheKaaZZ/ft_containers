/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 17:22:54 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/29 15:23:34 by bcosters         ###   ########.fr       */
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
#include <set>
#include <stack>
#include <type_traits>
#include <vector>
namespace ft = std;
#else
#include "../include/Iterators.hpp"
#include "../include/Map.hpp"
#include "../include/Set.hpp"
#include "../include/Stack.hpp"
#include "../include/Vector.hpp"
#include "../include/utility.hpp"
#endif // _IS_TEST
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <utility>

template <typename Value>
std::ostream &operator<<(std::ostream &os, ft::set<Value> const &m) {
  os << "{ ";
  for (typename ft::set<Value>::const_iterator it = m.begin(); it != m.end();
       it++)
    os << '(' << *it << ") ";
  return os << "}\n";
}

template <typename Value>
void print(const std::string &comment, ft::set<Value> const &m) {
  std::cout << comment;
  std::cout << m;
}

void set_tests() {
  std::cout << "\nCTOR\n";
  // (1) Default constructor
  ft::set<std::string> a;
  a.insert("cat");
  a.insert("dog");
  a.insert("horse");
  std::cout << a;

  // (2) Iterator constructor
  ft::set<std::string> b(a.find("dog"), a.end());
  std::cout << b;

  // (3) Copy constructor
  ft::set<std::string> c(a);
  c.insert("another horse");
  std::cout << c;

  std::cout << "\nOPERATOR=\n";
  ft::set<int> x, y;
  x.insert(1);
  x.insert(2);
  x.insert(3);

  std::cout << "Initially:\n";
  print("x = ", x);
  print("y = ", y);

  std::cout << "Copy assignment copies data from x to y:\n";
  y = x;
  print("x = ", x);
  print("y = ", y);

  std::cout << "\nGET_ALLOCATOR\n";
    ft::set<int> myset;
  int * p;
  unsigned int i;

  // allocate an array of 5 elements using myset's allocator:
  p=myset.get_allocator().allocate(5);

  // assign some values to array
  for (i=0; i<5; i++) p[i]=(i+1)*10;

  std::cout << "The allocated array contains:";
  for (i=0; i<5; i++) std::cout << ' ' << p[i];
  std::cout << '\n';

  myset.get_allocator().deallocate(p,5);

  std::cout << "\nBEGIN/END/RBEGIN/REND\n";
  int myints[] = {75,23,65,42,13};
  ft::set<int> myset2 (myints,myints+5);

  std::cout << "myset2 contains:";
  for (ft::set<int>::iterator it=myset2.begin(); it!=myset2.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
    ft::set<int>::reverse_iterator rit;

  std::cout << "myset contains:";
  for (rit=myset.rbegin(); rit != myset.rend(); ++rit)
    std::cout << ' ' << *rit;

  std::cout << '\n';

  std::cout << "\nEMPTY\n";
      ft::set<int> numbers;
    std::cout << std::boolalpha;
    std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';
 
    numbers.insert(42);
    numbers.insert(13317); 
    std::cout << "After adding elements, numbers.empty(): " << numbers.empty() << '\n';

  std::cout << "\nSIZE\n";
      ft::set<int> nums;
      nums.insert(1);
      nums.insert(3);
      nums.insert(5);
      nums.insert(7);
 
    std::cout << "nums contains " << nums.size() << " elements.\n";

  std::cout << "\nMAX_SIZE\n";
    //   ft::set<char> q;
    // std::cout.imbue(std::locale("en_US.UTF-8"));    
    // std::cout << "Maximum size of a ft::set is " << q.max_size() << '\n';
      if (myset.max_size()>1000)
  {
    for (i=0; i<1000; i++) myset.insert(i);
    std::cout << "The set contains 1000 elements.\n";
  }
  else std::cout << "The set could not hold 1000 elements.\n";

  std::cout << "\nCLEAR\n";
    myset.insert (100);
  myset.insert (200);
  myset.insert (300);

  std::cout << "myset contains:";
  for (ft::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  myset.clear();
  myset.insert (1101);
  myset.insert (2202);

  std::cout << "myset contains:";
  for (ft::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nINSERT\n";
    ft::set<int>::iterator it;
  ft::pair<ft::set<int>::iterator,bool> ret;

  // set some initial values:
  for (int i=1; i<=5; ++i) myset.insert(i*10);    // set: 10 20 30 40 50

  ret = myset.insert(20);               // no new element inserted

  if (ret.second==false) it=ret.first;  // "it" now points to element 20

  myset.insert (it,25);                 // max efficiency inserting
  myset.insert (it,24);                 // max efficiency inserting
  myset.insert (it,26);                 // no max efficiency inserting

  int myints2[]= {5,10,15};              // 10 already in set, not inserted
  myset.insert (myints2,myints2+3);

  std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nERASE\n";
    // insert some values:
  for (int i=1; i<10; i++) myset.insert(i*10);  // 10 20 30 40 50 60 70 80 90

  it = myset.begin();
  ++it;                                         // "it" points now to 20

  myset.erase (it);

  myset.erase (40);

  it = myset.find (60);
  myset.erase (it, myset.end());

  std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nSWAP\n";
  int myints3[] ={12,75,10,32,20,25};
    ft::set<int> first (myints3,myints3+3);     // 10,12,75
  ft::set<int> second (myints3+3,myints3+6);  // 20,25,32

  first.swap(second);

  std::cout << "first contains:";
  for (ft::set<int>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "second contains:";
  for (ft::set<int>::iterator it=second.begin(); it!=second.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nKEY_COMP\n";
    int highest;

  ft::set<int>::key_compare mycomp = myset.key_comp();

  for (int i=0; i<=5; i++) myset.insert(i);

  std::cout << "myset contains:";

  highest=*myset.rbegin();
  it=myset.begin();
  do {
    std::cout << ' ' << *it;
  } while ( mycomp(*(++it),highest) );

  std::cout << '\n';

  std::cout << "\nVALUE_COMP\n";
    ft::set<int>::value_compare mycomp2 = myset.value_comp();

  for (int i=0; i<=5; i++) myset.insert(i);

  std::cout << "myset contains:";

  highest=*myset.rbegin();
  it=myset.begin();
  do {
    std::cout << ' ' << *it;
  } while ( mycomp2(*(++it),highest) );

  std::cout << '\n';

  std::cout << "\nFIND\n";
    // set some initial values:
  for (int i=1; i<=5; i++) myset.insert(i*10);    // set: 10 20 30 40 50

  it=myset.find(20);
  myset.erase (it);
  myset.erase (myset.find(40));

  std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nCOUNT\n";
    // set some initial values:
  for (int i=1; i<5; ++i) myset.insert(i*3);    // set: 3 6 9 12

  for (int i=0; i<10; ++i)
  {
    std::cout << i;
    if (myset.count(i)!=0)
      std::cout << " is an element of myset.\n";
    else
      std::cout << " is not an element of myset.\n";
  }

  std::cout << "\nLOWER/UPPER_BOUND\n";
    ft::set<int>::iterator itlow,itup;

  for (int i=1; i<10; i++) myset.insert(i*10); // 10 20 30 40 50 60 70 80 90

  itlow=myset.lower_bound (30);                //       ^
  itup=myset.upper_bound (60);                 //                   ^

  myset.erase(itlow,itup);                     // 10 20 70 80 90

  std::cout << "myset contains:";
  for (ft::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "\nEQUAL_RANGE\n";
    for (int i=1; i<=5; i++) myset.insert(i*10);   // myset: 10 20 30 40 50

  ft::pair<ft::set<int>::const_iterator,ft::set<int>::const_iterator> ret2;
  ret2 = myset.equal_range(30);

  std::cout << "the lower bound points to: " << *ret2.first << '\n';
  std::cout << "the upper bound points to: " << *ret2.second << '\n';
}

int main() {
  set_tests();
  return 0;
}