#ifndef _IS_TEST
#include <map>
namespace ft = std;
#else
#include "../include/Map.hpp"
#endif

#include <iostream>

typedef std::string string;
typedef ft::map<int, char> mapp;
typedef ft::pair<int, char> pair;
typedef mapp::iterator iterator;
typedef mapp::const_iterator const_iterator;
typedef mapp::reverse_iterator reverse_iterator;
typedef mapp::const_reverse_iterator const_reverse_iterator;

int main()
{

	pair p[10];
	for (size_t i = 0; i < 10; i++)
	{
		p[i].first = i;
		p[i].second = 'a' + i;
	}

{
	mapp map(p, p + 10);

	std::cout << "[ -from- |iterator begin()| -to- |iterator end()| -not_included- ]" << std::endl;
	for (iterator it = map.begin(); it != map.end(); ++it)
	{
		std::cout << (*it).first << " - " << (*it).second << std::endl;
	}
	std::cout << "-----" << std::endl;
}
{
	mapp map(p, p + 10);

	std::cout << "[ -from- |const_iterator begin()| -to- |const_iterator end()| -not_included- ]" << std::endl;

	for (const_iterator it = map.begin(); it != map.end(); ++it)
	{
		std::cout << (*it).first << " - " << (*it).second << std::endl;
	}
	std::cout << "-----" << std::endl;
}
{
	mapp map(p, p + 10);

	std::cout << "[ -from- |reverse_iterator rbegin()| -to- |reverse_iterator rend()| -not_included- ]" << std::endl;

	for (reverse_iterator it = map.rbegin(); it != map.rend(); ++it)
	{
		std::cout << (*it).first << " - " << (*it).second << std::endl;
	}
	std::cout << "-----" << std::endl;
}
{
	mapp map(p, p + 10);

	std::cout << "[ -from- |const_reverse_iterator begin()| -to- |const_reverse_iterator end()| -not_included- ]" << std::endl;

	for (reverse_iterator it = map.rbegin(); it != map.rend(); ++it)
	{
		std::cout << (*it).first << " - " << (*it).second << std::endl;
	}
	std::cout << "-----" << std::endl;
}
	return 0;
}
