/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_tests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:53:16 by bcosters          #+#    #+#             */
/*   Updated: 2022/06/28 12:23:30 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IS_TEST
#include <algorithm>
// #include <bits/stdc++.h>
#include <iterator>
#include <map>
#include <memory>
#include <vector>
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
#include <functional>
#include <utility>

std::ostream& operator<<(std::ostream& s, const ft::vector<std::string>& v) 
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (ft::vector<std::basic_string<char> >::const_iterator it = v.begin(); it != v.end(); it++) {
        s << comma << *it;
        comma[0] = ',';
    }
    return s << ']';
}

std::ostream& operator<<(std::ostream& s, const ft::vector<int>& v) 
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (ft::vector<int>::const_iterator it = v.begin(); it != v.end(); it++) {
        s << comma << *it;
        comma[0] = ',';
    }
    return s << ']';
}

template<class T>
void print_vector(const ft::vector<T>& v) {
    typename ft::vector<T>::const_iterator it = v.begin();
    while (it != v.end()) {
        std::cout << *it++ << "\n";
    }
}

void vector_tests() {

    std::cout << "\nBASIC\n";
    // Create a vector containing integers
    ft::vector<int> v;
 
    // Add two more integers to vector
    v.push_back(25);
    v.push_back(13);
 
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
    std::cout << "Element at index 4 has value " << data[4]<< '\n';
 
    std::cout << "data size = " << data.size() << '\n';
 
    try {
        // Set element 6
        data.at(6) = 666;
    } catch (std::out_of_range const& exc) {
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
}

int main() {
  vector_tests();
  return 0;
}