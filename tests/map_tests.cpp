/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_tests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:45:21 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/28 12:03:46 by bcosters         ###   ########.fr       */
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

bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp {
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

void map_tests() {
  std::cout << "\nCTOR\n";
  std::cout << "Testing def ctor\n";

  ft::map<char,int> first;

  std::cout << "Testing operator[]\n";

  first['a']=10;
  first['b']=30;
  first['c']=50;
  first['d']=70;

  std::cout << "Testing range ctor\n";

  ft::map<char,int> second (first.begin(),first.end());

  std::cout << "Testing copt ctor\n";

  ft::map<char,int> third (second);

  std::cout << "Testing compare ctor\n";

  ft::map<char,int,classcomp> fourth;                 // class as Compare

  std::cout << "Testing compare func ptr ctor\n";

  bool(*fn_pt)(char,char) = fncomp;

  ft::map<char,int,bool(*)(char,char)> fifth (fn_pt); // function pointer as Compare
  std::cout << "Testing compare func ptr ctor\n";

  std::cout << "\nBEGIN/END/RBEGIN/REND\n";
  ft::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // show content:
  for (ft::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';
  
  // show content:
  ft::map<char,int>::reverse_iterator rit;
  for (rit=mymap.rbegin(); rit!=mymap.rend(); ++rit)
    std::cout << rit->first << " => " << rit->second << '\n';

  std::cout << "\nEMPTY\n";
  while (!mymap.empty())
  {
    std::cout << mymap.begin()->first << " => " << mymap.begin()->second << '\n';
    mymap.erase(mymap.begin());
  }

  std::cout << "\nSIZE\n";
  mymap['a']=101;
  mymap['b']=202;
  mymap['c']=302;

  std::cout << "mymap.size() is " << mymap.size() << '\n';

  std::cout << "\nMAX_SIZE\n";
  int i;
  ft::map<int,int> mymap2;
  if (mymap2.max_size()>1000)
  {
    for (i=0; i<1000; i++) mymap2[i]=0;
    std::cout << "The map contains 1000 elements.\n";
  }
  else std::cout << "The map could not hold 1000 elements.\n";

  std::cout << "\nOPERATOR[]\n";
  ft::map<char,std::string> mymap3;

  mymap3['a']="an element";
  mymap3['b']="another element";
  mymap3['c']=mymap3['b'];

  std::cout << "mymap3['a'] is " << mymap3['a'] << '\n';
  std::cout << "mymap3['b'] is " << mymap3['b'] << '\n';
  std::cout << "mymap3['c'] is " << mymap3['c'] << '\n';
  std::cout << "mymap3['d'] is " << mymap3['d'] << '\n';

  std::cout << "mymap3 now contains " << mymap3.size() << " elements.\n";

  std::cout << "\nAT\n";
  mymap3.at('a')="a new element";
  mymap3.at('b')="another new element";
  mymap3.at('c')=mymap3['b'];

  std::cout << "mymap3.at('a') is " << mymap3.at('a') << '\n';
  std::cout << "mymap3.at('b') is " << mymap3.at('b') << '\n';
  std::cout << "mymap3.at('c') is " << mymap3.at('c') << '\n';
  std::cout << "mymap3.at('d') is " << mymap3.at('d') << '\n';
  try {
  std::cout << "mymap3.at('e') is " << mymap3.at('e') << '\n';
  }
  catch(std::out_of_range const &exc) {
    std::cout << exc.what() << " exception\n";
  }

  std::cout << "mymap3 now contains " << mymap3.size() << " elements.\n";

  std::cout << "\nINSERT\n";
    // first insert function version (single parameter):
  mymap.insert ( ft::pair<char,int>('a',100) );
  mymap.insert ( ft::pair<char,int>('z',200) );

  ft::pair<ft::map<char,int>::iterator,bool> ret;
  ret = mymap.insert ( ft::pair<char,int>('z',500) );
  if (ret.second==false) {
    std::cout << "element 'z' already existed";
    std::cout << " with a value of " << ret.first->second << '\n';
  }

  // second insert function version (with hint position):
  ft::map<char,int>::iterator it = mymap.begin();
  mymap.insert (it, ft::pair<char,int>('b',300));  // max efficiency inserting
  mymap.insert (it, ft::pair<char,int>('c',400));  // no max efficiency inserting

  // third insert function version (range insertion):
  ft::map<char,int> anothermap;
  anothermap.insert(mymap.begin(),mymap.find('c'));

  // showing contents:
  std::cout << "mymap contains:\n";
  for (it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "anothermap contains:\n";
  for (it=anothermap.begin(); it!=anothermap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  std::cout << "\nERASE\n";
  ft::map<char,int>::iterator it2;

  // insert some values:
  mymap['a']=10;
  mymap['b']=20;
  mymap['c']=30;
  mymap['d']=40;
  mymap['e']=50;
  mymap['f']=60;

  it2=mymap.find('b');
  mymap.erase (it2);                   // erasing by it2erator

  mymap.erase ('c');                  // erasing by key

  it2=mymap.find ('e');
  mymap.erase ( it2, mymap.end() );    // erasing by range

  // show content:
  for (it2=mymap.begin(); it2!=mymap.end(); ++it2)
    std::cout << it2->first << " => " << it2->second << '\n';
}

int main() {
  map_tests();
  return 0;
}