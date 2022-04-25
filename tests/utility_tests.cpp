/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_tests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 17:07:51 by bcosters          #+#    #+#             */
/*   Updated: 2022/04/25 12:40:17 by bcosters         ###   ########.fr       */
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
#include <stdlib.h>
#include <vector>
#include <map>
#include <functional>
#include <utility>

template <unsigned n>
struct factorial : ft::integral_constant<int, n * factorial<n - 1>::value> {};

template <> struct factorial<0> : ft::integral_constant<int, 1> {};

template <typename T> void isintegral(T a) {
  (void)a;
  std::cout << std::boolalpha;
  std::cout << ft::is_integral<T>::value << std::endl;
}

struct base {
  int val;
};

struct derived : public base {};

struct trivial {
  int val;
};

bool mypredicate(int i, int j) { return (i == j); }

bool mycomp(char c1, char c2) { return std::tolower(c1) < std::tolower(c2); }

struct unreferenceable {
  int x;
  unreferenceable *operator&() { return NULL; }
};

void print(unreferenceable *m) {
  if (m)
    std::cout << m->x << '\n';
  else
    std::cout << "[null pointer]\n";
}

class cls {};

void util_tests() {
  // integral_constant
  std::cout << "\nINTEGRAL_CONSTANT\n";
  std::cout << "integral_constant<int, 5> == "
            << ft::integral_constant<int, 5>::value << std::endl;
  std::cout << "integral_constant<bool, false> == " << std::boolalpha
            << ft::integral_constant<bool, false>::value << std::endl;
  std::cout << factorial<5>::value << std::endl;
  // is_same
  std::cout << "\nIS_SAME\n";
  std::cout << "is_same<base, base> == " << std::boolalpha
            << ft::is_same<base, base>::value << std::endl;
  std::cout << "is_same<base, derived> == " << std::boolalpha
            << ft::is_same<base, derived>::value << std::endl;
  std::cout << "is_same<derived, base> == " << std::boolalpha
            << ft::is_same<derived, base>::value << std::endl;
  std::cout << "is_same<int, int> == " << std::boolalpha
            << ft::is_same<int, int>::value << std::endl;
  std::cout << "is_same<int, const int> == " << std::boolalpha
            << ft::is_same<int, const int>::value << std::endl;
  // is_integral
  std::cout << "\nIS_INTEGRAL\n";
  std::cout << "is_integral<trivial> == " << std::boolalpha
            << ft::is_integral<trivial>::value << std::endl;
  std::cout << "is_integral<int> == " << std::boolalpha
            << ft::is_integral<int>::value << std::endl;
  std::cout << "is_integral<float> == " << std::boolalpha
            << ft::is_integral<float>::value << std::endl;
  int a = 1;
  char b = 'h';
  long double c = 3.3333;
  double d = 1.23;
  bool e = true;
  float f = 0.22;

  isintegral(a);
  isintegral(b);
  isintegral(c);
  isintegral(d);
  isintegral(e);
  isintegral(f);
  // equal
  std::cout << "\nEQUAL COMPARE\n";
  int myints[] = {20, 40, 60, 80, 100};          //   myints: 20 40 60 80 100
  std::vector<int> myvector(myints, myints + 5); // myvector: 20 40 60 80 100

  // using default comparison:
  if (std::equal(myvector.begin(), myvector.end(), myints))
    std::cout << "The contents of both sequences are equal.\n";
  else
    std::cout << "The contents of both sequences differ.\n";

  myvector[3] = 81; // myvector: 20 40 60 81 100

  // using predicate comparison:
  if (std::equal(myvector.begin(), myvector.end(), myints, mypredicate))
    std::cout << "The contents of both sequences are equal.\n";
  else
    std::cout << "The contents of both sequences differ.\n";

  // lexicographical_compare
  std::cout << "\nLEXICOGRAPHICAL_COMPARE\n";
  char foo[] = "Apple";
  char bar[] = "apartment";

  std::cout << std::boolalpha;

  std::cout << "Comparing foo and bar lexicographically (foo<bar):\n";

  std::cout << "Using default comparison (operator<): ";
  std::cout << ft::lexicographical_compare(foo, foo + 5, bar, bar + 9);
  std::cout << '\n';

  std::cout << "Using mycomp as comparison object: ";
  std::cout << ft::lexicographical_compare(foo, foo + 5, bar, bar + 9, mycomp);
  std::cout << '\n';

  // remove_cv
  std::cout << "\nREMOVE_CV\n";
  std::cout << std::boolalpha;

  typedef ft::remove_cv<const int>::type type1;
  typedef ft::remove_cv<volatile int>::type type2;
  typedef ft::remove_cv<const volatile int>::type type3;
  typedef ft::remove_cv<const volatile int *>::type type4;
  typedef ft::remove_cv<int *const volatile>::type type5;

  std::cout << ft::is_same<type1, int>::value << "\n";
  std::cout << ft::is_same<type2, int>::value << "\n";
  std::cout << ft::is_same<type3, int>::value << "\n";
  std::cout << ft::is_same<type4, int *>::value << " "
            << ft::is_same<type4, const volatile int *>::value << "\n";
  std::cout << ft::is_same<type5, int *>::value << "\n";

  // is_floating_point
  std::cout << "\nIS_FLOATING_POINT\n";
  std::cout << std::boolalpha;
  std::cout << "trivial: " << ft::is_floating_point<trivial>::value << '\n';
  std::cout << "  float: " << ft::is_floating_point<float>::value << '\n';
  std::cout << " float&: " << ft::is_floating_point<float &>::value << '\n';
  std::cout << " double: " << ft::is_floating_point<double>::value << '\n';
  std::cout << "double&: " << ft::is_floating_point<double &>::value << '\n';
  std::cout << "    int: " << ft::is_floating_point<int>::value << '\n';

  // is_arithmetic
  std::cout << "\nIS_ARITHMETIC\n";
  std::cout
      << std::boolalpha << "trivial:     " << ft::is_arithmetic<trivial>::value
      << '\n'                                                           // false
      << "bool:        " << ft::is_arithmetic<bool>::value << '\n'      // true
      << "int:         " << ft::is_arithmetic<int>::value << '\n'       // true
      << "int const:   " << ft::is_arithmetic<int const>::value << '\n' // true
      << "int &:       " << ft::is_arithmetic<int &>::value << '\n'     // false
      << "int *:       " << ft::is_arithmetic<int *>::value << '\n'     // false
      << "float:       " << ft::is_arithmetic<float>::value << '\n'     // true
      << "float const: " << ft::is_arithmetic<float const>::value
      << '\n'                                                         // true
      << "float &:     " << ft::is_arithmetic<float &>::value << '\n' // false
      << "float *:     " << ft::is_arithmetic<float *>::value << '\n' // false
      << "char:        " << ft::is_arithmetic<char>::value << '\n'    // true
      << "char const:  " << ft::is_arithmetic<char const>::value << '\n' // true
      << "char &:      " << ft::is_arithmetic<char &>::value << '\n' // false
      << "char *:      " << ft::is_arithmetic<char *>::value << '\n' // false
      ;

  // is_member_pointer
  std::cout << "\nIS_MEMBER_POINTER\n";
  std::cout << (ft::is_member_pointer<int(cls::*)>::value
                    ? "T is member pointer"
                    : "T is not a member pointer")
            << '\n';
  std::cout << (ft::is_member_pointer<int>::value ? "T is member pointer"
                                                  : "T is not a member pointer")
            << '\n';

  // is_array
  std::cout << "\nIS_ARRAY\n";
  std::cout << std::boolalpha;
  std::cout << ft::is_array<trivial>::value << '\n';
  std::cout << ft::is_array<trivial[]>::value << '\n';
  std::cout << ft::is_array<trivial[3]>::value << '\n';
  std::cout << ft::is_array<float>::value << '\n';
  std::cout << ft::is_array<int>::value << '\n';
  std::cout << ft::is_array<int[]>::value << '\n';
  std::cout << ft::is_array<int[3]>::value << '\n';

  // is_array
  std::cout << "\nPAIR\n";\
  int n = 1;
    int y[5] = {1, 2, 3, 4, 5};
 
    // build a pair from two ints
    ft::pair<int, int> p = ft::make_pair(n, y[1]);
    std::cout << "The value of p1 is "
              << "(" << p.first << ", " << p.second << ")\n";

	// Using the constructor to declare and initialize a pair
   ft::pair <int, double> p1 ( 10, 1.1e-2 );

   // Compare using the helper function to declare and initialize a pair
   ft::pair <int, double> p2;
   p2 = ft::make_pair ( 10, 2.22e-1 );

   // Making a copy of a pair
   ft::pair <int, double> p3 ( p1 );

   std::cout.precision ( 3 );
   std::cout << "The pair p1 is: ( " << p1.first << ", "
        << p1.second << " )." << std::endl;
   std::cout << "The pair p2 is: ( " << p2.first << ", "
        << p2.second << " )." << std::endl;
   std::cout << "The pair p3 is: ( " << p3.first << ", "
        << p3.second << " )." << std::endl;

}

int main() {
  util_tests();
  return 0;
}