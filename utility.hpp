/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 12:07:37 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/11 12:15:05 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

namespace ft {

    //  integral_constant //
    //---------------------------------------//
template <class T, T v>
struct integral_constant {
	static constexpr T value = v;
	typedef T						value_type;
	typedef integral_constant<T,v>	type;
	constexpr operator value_type() { return value; }
};

typedef integral_constant<bool, true>	true_type;
typedef integral_constant<bool, false>	false_type;

    //  is_same //
    //---------------------------------------//
//operator bool inherited from integral_constant
// !=
template<class T, class U>
struct is_same : false_type {};
// ==
template<class T>
struct is_same<T, T> : true_type {};

    //  is_integral //
    //---------------------------------------//
//Check if type is integral
template<bool>
struct is_integral : true_type {};
template<char>
struct is_integral : true_type {};
template<char16_t>
struct is_integral : true_type {};
template<char32_t>
struct is_integral : true_type {};
template<wchar_t>
struct is_integral : true_type {};
template<signed char>
struct is_integral : true_type {};
template<short int>
struct is_integral : true_type {};
template<int>
struct is_integral : true_type {};
template<long int>
struct is_integral : true_type {};
template<long long int>
struct is_integral : true_type {};
template<unsigned char>
struct is_integral : true_type {};
template<unsigned short int>
struct is_integral : true_type {};
template<unsigned int>
struct is_integral : true_type {};
template<unsigned long int>
struct is_integral : true_type {};
template<unsigned long long int>
struct is_integral : true_type {};
//none of the above => NOT integral
template<class T>
struct is_integral : false_type {};

    //  enable_if //
    //---------------------------------------//
//Primary template
template<bool Cond, class T = void>
struct enable_if {};
//Specialization for TRUE
template<class T>
struct enable_if<true, T> {
	typedef T type;
};

    //  equal_compare //
    //---------------------------------------//
//Test whether the elements in two ranges are equal
//Compares the elements in the range [first1,last1) with those in the range beginning at first2,
//and returns true if all of the elements in both ranges match.
template <class InputIterator1, class InputIterator2>
bool	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
	while (first1 != last1) {
		if (!(*first1 == *first2))   // or: if (!pred(*first1,*first2)), for version 2
			return false;
		++first1;
		++first2;
  	}
	return true;
}

    //  lexographical_compare //
    //---------------------------------------//
//Returns true if the range [first1,last1) compares lexicographically less than the range [first2,last2).
//A lexicographical comparison is the kind of comparison
//-> generally used to sort words alphabetically in dictionaries;
//It involves comparing sequentially the elements that have the same position in both ranges
//against each other until one element is not equivalent to the other.
//The result of comparing these first non-matching elements is the result of the lexicographical comparison.
//If both sequences compare equal until one of them ends,
//the shorter sequence is lexicographically less than the longer one.
template <class InputIterator1, class InputIterator2>
bool	lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1)
	{
		if (first2 == last2 || *first2 < *first1) return false;
		else if (*first1 < *first2) return true;
		++first1;
		++first2;
	}
	return (first2!=last2);
}


    //  pair //
    //---------------------------------------//
template<class T1, class T2>
struct pair {
	typedef typename T1	first_type;
	typedef typename T2	second_type;
	first_type	first;
	second_type	second;

	pair() : first(), second() {};
	template<class T, class U>
	pair(pair<T,U> const & src) : first(src.first), second(src.second) {};
	pair(first_type const & a, second_type const & b) : first(a), second(b) {};

	pair&	operator=(pair const & rhs) {
		if (*this != rhs) {
			first = rhs.first;
			second = rhs.second;
		}
		return *this;
	}
    //  pair - relational operators //
	friend bool	operator==(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
	friend bool	operator!=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs) {
		return !(lhs == rhs);
	}
	friend bool	operator<(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs) {
		return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
	}
	friend bool	operator<=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs) {
		return !(rhs<lhs);
	}
	friend bool	operator>(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs) {
		return rhs<lhs;
	}
	friend bool	operator>=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs) {
		return !(lhs<rhs);
}
};

    //  pair - make_pair //
template <class T1,class T2>
pair<T1,T2>		make_pair(T1 x, T2 y) {
	return (pair<T1,T2>(x, y));
}

}
#endif