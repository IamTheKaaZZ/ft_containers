/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_tests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 17:07:51 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/14 17:50:15 by bcosters         ###   ########.fr       */
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
# include <vector>
#include <iostream>
#include <stdlib.h>

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

void	util_tests() {
	//integral_constant
	std::cout << "INTEGRAL_CONSTANT\n";
	std::cout << "integral_constant<int, 5> == "
        << ft::integral_constant<int, 5>::value << std::endl;
    std::cout << "integral_constant<bool, false> == " << std::boolalpha
        << ft::integral_constant<bool, false>::value << std::endl;
	//is_same
	std::cout << "IS_SAME\n";
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
	std::cout << "IS_INTEGRAL\n";
	std::cout << "is_integral<trivial> == " << std::boolalpha
        << ft::is_integral<trivial>::value << std::endl;
    std::cout << "is_integral<int> == " << std::boolalpha
        << ft::is_integral<int>::value << std::endl;
    std::cout << "is_integral<float> == " << std::boolalpha
        << ft::is_integral<float>::value << std::endl;
	//equal
	std::cout << "EQUAL COMPARE\n";
	int v1[] = { 10, 20, 30, 40, 50 };
    std::vector<int> vector_1 (v1, v1 + sizeof(v1) / sizeof(int) );
  
    std::cout << "Vector contains : ";
    for (unsigned int i = 0; i < vector_1.size(); i++)
        std::cout << " " << vector_1[i];
    std::cout << "\n";
  
    if ( ft::equal (vector_1.begin(), vector_1.end(), v1) )
        std::cout << "The contents of both sequences are equal.\n";
    else
        std::cout <<"The contents of both sequences differ.\n";
}

int	main() {
	util_tests();
	return 0;
}