/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_tests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:45:21 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/28 16:21:24 by bcosters         ###   ########.fr       */
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
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <utility>

bool fncomp(char lhs, char rhs) { return lhs < rhs; }

struct classcomp {
  bool operator()(const char &lhs, const char &rhs) const { return lhs < rhs; }
};

template <typename Key, typename Value>
std::ostream &operator<<(std::ostream &os, ft::map<Key, Value> const &m) {
  os << "{ ";
  for (typename ft::map<Key, Value>::const_iterator it = m.begin();
       it != m.end(); it++)
    os << '(' << it->first << ':' << it->second << ") ";
  return os << "}\n";
}

template <typename Key, typename Value>
void print(const std::string &comment, ft::map<Key, Value> const &m) {
  std::cout << comment;
  std::cout << m;
}

struct Point {
  double x, y;
  Point() : x(0), y(0) {}
  Point(double x, double y) : x(x), y(y) {}
};
struct PointCmp {
  bool operator()(const Point &lhs, const Point &rhs) const {
    return lhs.x < rhs.x; // NB. intentionally ignores y
  }
  bool operator()(const Point *lhs, const Point *rhs) const {
    return lhs->x < rhs->x; // NB. intentionally ignores y
  }
};

template <typename It> void printInsertionStatus(It it, bool success) {
  std::cout << "Insertion of " << it->first
            << (success ? " succeeded\n" : " failed\n");
}

void map_tests() {
  std::cout << "\nCTOR\n";
  std::cout << "Testing def ctor\n";

  ft::map<char, int> first;

  std::cout << "Testing operator[]\n";

  first['a'] = 10;
  first['b'] = 30;
  first['c'] = 50;
  first['d'] = 70;

  std::cout << "Testing range ctor\n";

  ft::map<char, int> second(first.begin(), first.end());

  std::cout << "Testing copt ctor\n";

  ft::map<char, int> third(second);

  std::cout << "Testing compare ctor\n";

  ft::map<char, int, classcomp> fourth; // class as Compare

  std::cout << "Testing compare func ptr ctor\n";

  bool (*fn_pt)(char, char) = fncomp;

  ft::map<char, int, bool (*)(char, char)> fifth(
      fn_pt); // function pointer as Compare
  std::cout << "Testing compare func ptr ctor\n";

  // (1) Default constructor
  ft::map<std::string, int> map1;
  map1["something"] = 69;
  map1["anything"] = 199;
  map1["that thing"] = 50;
  std::cout << "map1 = " << map1;

  // (4) Range constructor
  ft::map<std::string, int> iter(map1.find("anything"), map1.end());
  std::cout << "\niter = " << iter;
  std::cout << "map1 = " << map1;

  // (6) Copy constructor
  ft::map<std::string, int> copied(map1);
  std::cout << "\ncopied = " << copied;
  std::cout << "map1 = " << map1;

  std::cout << "\nCustom Key class option 1:\n";
  // Use a comparison struct
  ft::map<Point, double, PointCmp> mag;
  mag.insert(ft::pair<Point, double>(Point(5, -12), 13));
  mag.insert(ft::pair<Point, double>(Point(3, 4), 5));
  mag.insert(ft::pair<Point, double>(Point(-8, -15), 17));

  for (ft::map<Point, double, PointCmp>::iterator it = mag.begin();
       it != mag.end(); it++)
    std::cout << "The magnitude of (" << it->first.x << ", " << it->first.y
              << ") is " << it->second << '\n';

  std::cout << "\nOPERATOR=\n";
  ft::map<int, int> x, y;
  x[1] = 1;
  x[2] = 2;
  x[3] = 3;

  std::cout << "Initially:\n";
  print("x = ", x);
  print("y = ", y);

  std::cout << "Copy assignment copies data from x to y:\n";
  y = x;
  print("x = ", x);
  print("y = ", y);

  std::cout << "\nBEGIN/END/RBEGIN/REND\n";
  ft::map<char, int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // show content:
  for (ft::map<char, int>::iterator it = mymap.begin(); it != mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  // show content:
  ft::map<char, int>::reverse_iterator rit;
  for (rit = mymap.rbegin(); rit != mymap.rend(); ++rit)
    std::cout << rit->first << " => " << rit->second << '\n';

  // Note that although the x-coordinates are out of order, the
  // map will be iterated through by increasing x-coordinates
  Point *points = new Point[3];
  points[0] = Point(2, 0);
  points[1] = Point(1, 0);
  points[2] = Point(3, 0);

  // mag is a map sending the address of node to its magnitude in the x-y plane
  // Although the keys are pointers-to-Point, we want to order the map by the
  // x-coordinates of the points and NOT by the addresses of the Points. This
  // is done by using the PointCmp class's comparison method.
  ft::map<Point *, double, PointCmp> mag2;
  mag2.insert(ft::pair<Point *, double>(points, 2));
  mag2.insert(ft::pair<Point *, double>(points + 1, 1));
  mag2.insert(ft::pair<Point *, double>(points + 2, 3));

  // Change each y-coordinate from 0 to the mag2nitude
  for (ft::map<Point *, double, PointCmp>::iterator iter = mag2.begin();
       iter != mag2.end(); ++iter) {
    Point *cur = iter->first; // pointer to Node
    cur->y = iter->second;    // could also have used  cur->y = iter->second;
  }

  // Update and print the mag2nitude of each node
  for (ft::map<Point *, double, PointCmp>::iterator iter = mag2.begin();
       iter != mag2.end(); ++iter) {
    Point *cur = iter->first;
    mag2[cur] = hypot(cur->x, cur->y);
    std::cout << "The magnitude of (" << cur->x << ", " << cur->y << ") is ";
    std::cout << iter->second << '\n';
  }

  delete[] points;

  std::cout << "\nEMPTY\n";
  while (!mymap.empty()) {
    std::cout << mymap.begin()->first << " => " << mymap.begin()->second
              << '\n';
    mymap.erase(mymap.begin());
  }

  std::cout << "\nSIZE\n";
  mymap['a'] = 101;
  mymap['b'] = 202;
  mymap['c'] = 302;

  std::cout << "mymap.size() is " << mymap.size() << '\n';

  std::cout << "\nMAX_SIZE\n";
  int i;
  ft::map<int, int> mymap2;
  if (mymap2.max_size() > 1000) {
    for (i = 0; i < 1000; i++)
      mymap2[i] = 0;
    std::cout << "The map contains 1000 elements.\n";
  } else
    std::cout << "The map could not hold 1000 elements.\n";

  std::cout << "\nOPERATOR[]\n";
  ft::map<char, std::string> mymap3;

  mymap3['a'] = "an element";
  mymap3['b'] = "another element";
  mymap3['c'] = mymap3['b'];

  std::cout << "mymap3['a'] is " << mymap3['a'] << '\n';
  std::cout << "mymap3['b'] is " << mymap3['b'] << '\n';
  std::cout << "mymap3['c'] is " << mymap3['c'] << '\n';
  std::cout << "mymap3['d'] is " << mymap3['d'] << '\n';

  std::cout << "mymap3 now contains " << mymap3.size() << " elements.\n";

  std::cout << "\nAT\n";
  mymap3.at('a') = "a new element";
  mymap3.at('b') = "another new element";
  mymap3.at('c') = mymap3['b'];

  std::cout << "mymap3.at('a') is " << mymap3.at('a') << '\n';
  std::cout << "mymap3.at('b') is " << mymap3.at('b') << '\n';
  std::cout << "mymap3.at('c') is " << mymap3.at('c') << '\n';
  std::cout << "mymap3.at('d') is " << mymap3.at('d') << '\n';
  try {
    std::cout << "mymap3.at('e') is " << mymap3.at('e') << '\n';
  } catch (std::out_of_range const &exc) {
    std::cout << exc.what() << " exception\n";
  }

  std::cout << "mymap3 now contains " << mymap3.size() << " elements.\n";

  std::cout << "\nINSERT\n";
  // first insert function version (single parameter):
  mymap.insert(ft::pair<char, int>('a', 100));
  mymap.insert(ft::pair<char, int>('z', 200));

  ft::pair<ft::map<char, int>::iterator, bool> ret2;
  ret2 = mymap.insert(ft::pair<char, int>('z', 500));
  if (ret2.second == false) {
    std::cout << "element 'z' already existed";
    std::cout << " with a value of " << ret2.first->second << '\n';
  }

  // second insert function version (with hint position):
  ft::map<char, int>::iterator it = mymap.begin();
  mymap.insert(it, ft::pair<char, int>('b', 300)); // max efficiency inserting
  mymap.insert(it,
               ft::pair<char, int>('c', 400)); // no max efficiency inserting

  // third insert function version (range insertion):
  ft::map<char, int> anothermap;
  anothermap.insert(mymap.begin(), mymap.find('c'));

  // showing contents:
  std::cout << "mymap contains:\n";
  for (it = mymap.begin(); it != mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "anothermap contains:\n";
  for (it = anothermap.begin(); it != anothermap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "\nERASE\n";
  ft::map<char, int>::iterator it2;

  // insert some values:
  mymap['a'] = 10;
  mymap['b'] = 20;
  mymap['c'] = 30;
  mymap['d'] = 40;
  mymap['e'] = 50;
  mymap['f'] = 60;

  it2 = mymap.find('b');
  mymap.erase(it2); // erasing by it2erator

  mymap.erase('c'); // erasing by key

  it2 = mymap.find('e');
  mymap.erase(it2, mymap.end()); // erasing by range

  // show content:
  for (it2 = mymap.begin(); it2 != mymap.end(); ++it2)
    std::cout << it2->first << " => " << it2->second << '\n';

  std::cout << "\nSWAP\n";
  ft::map<char, int> foo, bar;

  foo['x'] = 100;
  foo['y'] = 200;

  bar['a'] = 11;
  bar['b'] = 22;
  bar['c'] = 33;

  std::cout << "foo contains:\n";
  for (ft::map<char, int>::iterator it = foo.begin(); it != foo.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "bar contains:\n";
  for (ft::map<char, int>::iterator it = bar.begin(); it != bar.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  foo.swap(bar);

  std::cout << "foo contains:\n";
  for (ft::map<char, int>::iterator it = foo.begin(); it != foo.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "bar contains:\n";
  for (ft::map<char, int>::iterator it = bar.begin(); it != bar.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "\nCLEAR\n";
  mymap['x'] = 100;
  mymap['y'] = 200;
  mymap['z'] = 300;

  std::cout << "mymap contains:\n";
  for (ft::map<char, int>::iterator it = mymap.begin(); it != mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  mymap.clear();
  mymap['a'] = 1101;
  mymap['b'] = 2202;

  std::cout << "mymap contains:\n";
  for (ft::map<char, int>::iterator it = mymap.begin(); it != mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "\nKEY_COMP\n";
  ft::map<char, int>::key_compare mycomp = mymap.key_comp();

  mymap['a'] = 100;
  mymap['b'] = 200;
  mymap['c'] = 300;

  std::cout << "mymap contains:\n";

  char highest = mymap.rbegin()->first; // key value of last element

  ft::map<char, int>::iterator it3 = mymap.begin();
  do {
    std::cout << it3->first << " => " << it3->second << '\n';
  } while (mycomp((*it3++).first, highest));

  std::cout << '\n';

  std::cout << "\nVALUE_COMP\n";
  mymap['x'] = 1001;
  mymap['y'] = 2002;
  mymap['z'] = 3003;

  std::cout << "mymap contains:\n";

  ft::pair<char, int> highest2 = *mymap.rbegin(); // last element

  ft::map<char, int>::iterator it4 = mymap.begin();
  do {
    std::cout << it4->first << " => " << it4->second << '\n';
  } while (mymap.value_comp()(*it4++, highest2));

  std::cout << "\nFIND\n";
  mymap['a'] = 50;
  mymap['b'] = 100;
  mymap['c'] = 150;
  mymap['d'] = 200;

  it = mymap.find('b');
  if (it != mymap.end())
    mymap.erase(it);

  // print content:
  std::cout << "elements in mymap:" << '\n';
  std::cout << "a => " << mymap.find('a')->second << '\n';
  std::cout << "c => " << mymap.find('c')->second << '\n';
  std::cout << "d => " << mymap.find('d')->second << '\n';

  std::cout << "\nCOUNT\n";
  char c;

  mymap['a'] = 101;
  mymap['c'] = 202;
  mymap['f'] = 303;

  for (c = 'a'; c < 'h'; c++) {
    std::cout << c;
    if (mymap.count(c) > 0)
      std::cout << " is an element of mymap.\n";
    else
      std::cout << " is not an element of mymap.\n";
  }

  std::cout << "\nLOWER/UPPER_BOUND\n";
  ft::map<char, int>::iterator itlow, itup;

  mymap['a'] = 20;
  mymap['b'] = 40;
  mymap['c'] = 60;
  mymap['d'] = 80;
  mymap['e'] = 100;

  itlow = mymap.lower_bound('b'); // itlow points to b
  itup = mymap.upper_bound('d');  // itup points to e (not d!)

  mymap.erase(itlow, itup); // erases [itlow,itup)

  // print content:
  for (ft::map<char, int>::iterator it = mymap.begin(); it != mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "\nEQUAL_RANGE\n";
  mymap['a'] = 10;
  mymap['b'] = 20;
  mymap['c'] = 30;

  ft::pair<ft::map<char, int>::iterator, ft::map<char, int>::iterator> ret;
  ret = mymap.equal_range('b');

  std::cout << "lower bound points to: ";
  std::cout << ret.first->first << " => " << ret.first->second << '\n';

  std::cout << "upper bound points to: ";
  std::cout << ret.second->first << " => " << ret.second->second << '\n';

  std::cout << "\nGET_ALLOCATOR\n";
  int psize;
  ft::pair<const char, int> *p;

  // allocate an array of 5 elements using mymap's allocator:
  p = mymap.get_allocator().allocate(5);

  // assign some values to array
  psize = sizeof(ft::map<char, int>::value_type) * 5;

  std::cout << "The allocated array has a size of " << psize << " bytes.\n";

  mymap.get_allocator().deallocate(p, 5);
}

int main() {
  map_tests();
  return 0;
}