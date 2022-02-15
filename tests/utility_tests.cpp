/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_tests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 17:07:51 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/15 11:33:06 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IS_TEST
# include <type_traits>
# include <map>
# include <stack>
# include <iterator>
# include <algorithm>
# include <bits/stdc++.h>
namespace ft = std;
#else
# include "../include/Iterators.hpp"
# include "../include/Map.hpp"
# include "../include/Stack.hpp"
# include "../include/Vector.hpp"
# include "../include/utility.hpp"
#endif // _IS_TEST
#include <vector>
#include <iostream>
#include <stdlib.h>

template <unsigned n>
struct factorial : ft::integral_constant<int,n * factorial<n-1>::value> {};

template <>
struct factorial<0> : ft::integral_constant<int,1> {};

template <typename T> 
void isintegral(T a){
	(void)a;
    std::cout << std::boolalpha;
    std::cout << ft::is_integral<T>::value << std::endl;
}

struct base
    {
    int val;
    };

struct derived
    : public base
    {
    };

struct trivial
    {
    int val;
    };

bool mypredicate (int i, int j) {
  return (i==j);
}

bool mycomp (char c1, char c2)
{ return std::tolower(c1)<std::tolower(c2); }

void	util_tests() {
	//integral_constant
	std::cout << "\nINTEGRAL_CONSTANT\n";
	std::cout << "integral_constant<int, 5> == "
        << ft::integral_constant<int, 5>::value << std::endl;
    std::cout << "integral_constant<bool, false> == " << std::boolalpha
        << ft::integral_constant<bool, false>::value << std::endl;
	std::cout << factorial<5>::value << std::endl;
	//is_same
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
	//is_integral
	std::cout << "\nIS_INTEGRAL\n";
	std::cout << "is_integral<trivial> == " << std::boolalpha
        << ft::is_integral<trivial>::value << std::endl;
    std::cout << "is_integral<int> == " << std::boolalpha
        << ft::is_integral<int>::value << std::endl;
    std::cout << "is_integral<float> == " << std::boolalpha
        << ft::is_integral<float>::value << std::endl;
	int			a = 1;
    char		b = 'h';
    long double c = 3.3333;
    double		d = 1.23;
    bool		e = true;
    float		f = 0.22;
    
    isintegral(a);
    isintegral(b);
    isintegral(c);
    isintegral(d);
    isintegral(e);
    isintegral(f);
	//equal
	std::cout << "\nEQUAL COMPARE\n";
	int myints[] = {20,40,60,80,100};               //   myints: 20 40 60 80 100
  	std::vector<int>myvector (myints,myints+5);     // myvector: 20 40 60 80 100

  	// using default comparison:
  	if ( std::equal (myvector.begin(), myvector.end(), myints) )
    	std::cout << "The contents of both sequences are equal.\n";
  	else
    	std::cout << "The contents of both sequences differ.\n";

  	myvector[3]=81;                                 // myvector: 20 40 60 81 100

  	// using predicate comparison:
  	if ( std::equal (myvector.begin(), myvector.end(), myints, mypredicate) )
    	std::cout << "The contents of both sequences are equal.\n";
  	else
    	std::cout << "The contents of both sequences differ.\n";

	//lexicographical_compare
	std::cout << "\nLEXICOGRAPHICAL_COMPARE\n";
	char foo[]="Apple";
  	char bar[]="apartment";

  	std::cout << std::boolalpha;

  	std::cout << "Comparing foo and bar lexicographically (foo<bar):\n";

  	std::cout << "Using default comparison (operator<): ";
  	std::cout << ft::lexicographical_compare(foo,foo+5,bar,bar+9);
  	std::cout << '\n';

  	std::cout << "Using mycomp as comparison object: ";
  	std::cout << ft::lexicographical_compare(foo,foo+5,bar,bar+9,mycomp);
  	std::cout << '\n';
}

int	main() {
	util_tests();
	return 0;
}