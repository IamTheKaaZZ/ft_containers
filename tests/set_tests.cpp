/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 17:22:54 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/28 17:28:23 by bcosters         ###   ########.fr       */
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
#include <set>
namespace ft = std;
#else
#include "../include/Iterators.hpp"
#include "../include/Map.hpp"
#include "../include/Stack.hpp"
#include "../include/Vector.hpp"
#include "../include/Set.hpp"
#include "../include/utility.hpp"
#endif // _IS_TEST
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <utility>
#include <deque>

template <typename Value>
std::ostream &operator<<(std::ostream &os, ft::set<Value> const &m) {
  os << "{ ";
  for (typename ft::set<Value>::const_iterator it = m.begin();
       it != m.end(); it++)
    os << '(' << *it  << ") ";
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

}

int main() {
    set_tests();
    return 0;
}