/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_tests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:53:16 by bcosters          #+#    #+#             */
/*   Updated: 2022/06/27 17:32:49 by bcosters         ###   ########.fr       */
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
#include "../include/Vector.hpp"
#include "../include/utility.hpp"
#endif // _IS_TEST
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <functional>
#include <utility>

std::ostream& operator<<(std::ostream& s, const std::vector<std::string>& v) 
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (std::vector<std::basic_string<char> >::const_iterator it = v.begin(); it != v.end(); it++) {
        s << comma << *it;
        comma[0] = ',';
    }
    return s << ']';
}

std::ostream& operator<<(std::ostream& s, const std::vector<int>& v) 
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); it++) {
        s << comma << *it;
        comma[0] = ',';
    }
    return s << ']';
}

template<class T>
void print_vector(const std::vector<T>& v) {
    typename std::vector<T>::const_iterator it = v.begin();
    while (it != v.end()) {
        std::cout << *it++ << "\n";
    }
}

void vector_tests() {

    // BASIC
    // Create a vector containing integers
    std::vector<int> v;
 
    // Add two more integers to vector
    v.push_back(25);
    v.push_back(13);
 
    // Print out the vector
    std::cout << "v = { ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        std::cout << *it << ", ";
    }
    std::cout << "}; \n";

    // CTOR
    std::vector<std::string> words1;
    words1.push_back("the");
    words1.push_back("frogurt");
    words1.push_back("is");
    words1.push_back("also");
    words1.push_back("cursed");

    std::cout << "words1: " << words1 << '\n';
 
    // words2 == words1
    std::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';
 
    // words3 == words1
    std::vector<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';
 
    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    std::vector<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';

    // OPERATOR=
    std::vector<int> x, y;
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

    // ASSIGN
    std::vector<char> characters;
 
    characters.assign(5, 'a');
    print_vector(characters);
 
    const std::string extra(6, 'b');
    characters.assign(extra.begin(), extra.end());
    print_vector(characters);

    std::vector<int> data;
    data.push_back(1);
    data.push_back(2);
    data.push_back(3);
    data.push_back(1);
    data.push_back(2);
    data.push_back(3);
 
    // AT
    // Set element 1
    data.at(1) = 88;
 
    // Read element 2
    std::cout << "Element at index 2 has value " << data.at(2) << '\n';
 
    std::cout << "data size = " << data.size() << '\n';
 
    try {
        // Set element 6
        data.at(6) = 666;
    } catch (std::out_of_range const& exc) {
        std::cout << exc.what() << '\n';
    }
 
    // Print final values
    std::cout << "data:" << data;
    std::cout << '\n';
}

int main() {
  vector_tests();
  return 0;
}