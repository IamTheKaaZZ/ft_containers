/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 12:07:37 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/15 15:04:43 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <cstddef>

namespace ft {

    //  integral_constant //
    //---------------------------------------//
template <class T, T v>
struct integral_constant {
	static const T					value = v;
	typedef T						value_type;
	typedef integral_constant<T,v>	type;
	operator value_type() { return value; }
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
//none of the below => NOT integral
template<class T>
struct is_integral : false_type {};
//Check if type is integral (including const/volatile qualifications)
//	=> specializations of the template
template<>
struct is_integral<bool> : true_type {};
template<>
struct is_integral<const bool> : true_type {};
template<>
struct is_integral<const volatile bool> : true_type {};
template<>
struct is_integral<char> : true_type {};
template<>
struct is_integral<const char> : true_type {};
template<>
struct is_integral<const volatile char> : true_type {};
template<>
struct is_integral<wchar_t> : true_type {};
template<>
struct is_integral<const wchar_t> : true_type {};
template<>
struct is_integral<const volatile wchar_t> : true_type {};
template<>
struct is_integral<signed char> : true_type {};
template<>
struct is_integral<const signed char> : true_type {};
template<>
struct is_integral<const volatile signed char> : true_type {};
template<>
struct is_integral<short int> : true_type {};
template<>
struct is_integral<const short int> : true_type {};
template<>
struct is_integral<const volatile short int> : true_type {};
template<>
struct is_integral<int> : true_type {};
template<>
struct is_integral<const int> : true_type {};
template<>
struct is_integral<const volatile int> : true_type {};
template<>
struct is_integral<long int> : true_type {};
template<>
struct is_integral<const long int> : true_type {};
template<>
struct is_integral<const volatile long int> : true_type {};
template<>
struct is_integral<long long int> : true_type {};
template<>
struct is_integral<const long long int> : true_type {};
template<>
struct is_integral<const volatile long long int> : true_type {};
template<>
struct is_integral<unsigned char> : true_type {};
template<>
struct is_integral<const unsigned char> : true_type {};
template<>
struct is_integral<const volatile unsigned char> : true_type {};
template<>
struct is_integral<unsigned short int> : true_type {};
template<>
struct is_integral<const unsigned short int> : true_type {};
template<>
struct is_integral<const volatile unsigned short int> : true_type {};
template<>
struct is_integral<unsigned int> : true_type {};
template<>
struct is_integral<const unsigned int> : true_type {};
template<>
struct is_integral<const volatile unsigned int> : true_type {};
template<>
struct is_integral<unsigned long int> : true_type {};
template<>
struct is_integral<const unsigned long int> : true_type {};
template<>
struct is_integral<const volatile unsigned long int> : true_type {};
template<>
struct is_integral<unsigned long long int> : true_type {};
template<>
struct is_integral<const unsigned long long int> : true_type {};
template<>
struct is_integral<const volatile unsigned long long int> : true_type {};

    //  remove_cv //
    //---------------------------------------//
//Remove const/volatile from type
template< class T > struct remove_cv                   { typedef T type; };
template< class T > struct remove_cv<const T>          { typedef T type; };
template< class T > struct remove_cv<volatile T>       { typedef T type; };
template< class T > struct remove_cv<const volatile T> { typedef T type; };

    //  is_floating_point //
    //---------------------------------------//
template< class T >
struct is_floating_point
     : integral_constant<
         bool,
         is_same<float, typename remove_cv<T>::type>::value  ||
         is_same<double, typename remove_cv<T>::type>::value  ||
         is_same<long double, typename remove_cv<T>::type>::value
     > {};

    //  is_arithmetic //
    //---------------------------------------//
template< class T >
struct is_arithmetic : integral_constant<bool,
        is_integral<T>::value || is_floating_point<T>::value> {};

//Not implementing is_enum

    //  is_pointer //
    //---------------------------------------//
template<class T>
struct is_pointer : false_type {};
template<class T>
struct is_pointer<T*> : true_type {};
template<class T>
struct is_pointer<T* const> : true_type {};
template<class T>
struct is_pointer<T* volatile> : true_type {};
template<class T>
struct is_pointer<T* const volatile> : true_type {};

    //  is_member_pointer //
    //---------------------------------------//
template< class T >
struct is_member_pointer_helper         : false_type {};
template< class T, class U >
struct is_member_pointer_helper<T U::*> : true_type {};
template< class T >
struct is_member_pointer : 
    is_member_pointer_helper<typename remove_cv<T>::type> {};

    //  is_scalar //
    //---------------------------------------//
template< class T >
struct is_scalar : integral_constant<bool,
        			is_arithmetic<T>::value     ||
                    is_pointer<T>::value        ||
                    is_member_pointer<T>::value> {};

    //  is_array //
    //---------------------------------------//
template<class T>
struct is_array : false_type {};
template<class T>
struct is_array<T[]> : true_type {};
template<class T, std::size_t N>
struct is_array<T[N]> : true_type {};

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
template <class InputIt1, class InputIt2>
bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
	while (first1 != last1) {
		if (!(*first1 == *first2))
			return false;
		++first1;
		++first2;
  	}
	return true;
}

/*
binary predicate which returns ​true if the elements should be treated as equal.
The signature of the predicate function should be equivalent to the following:

 bool pred(const Type1 &a, const Type2 &b);

While the signature does not need to have const &,
the function must not modify the objects passed to it and must be able to accept all values of type
(possibly const) Type1 and Type2 regardless of value category (thus, Type1 & is not allowed).
*/
template <class InputIt1, class InputIt2, class BinaryPredicate>
bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
	while (first1 != last1) {
		if (!p(*first1,*first2))
			return false;
		++first1;
		++first2;
  	}
	return true;
}

    //  lexographical_compare //
    //---------------------------------------//
/*
Returns true if the range [first1,last1) compares lexicographically less than the range [first2,last2).
A lexicographical comparison is the kind of comparison
-> generally used to sort words alphabetically in dictionaries;
It involves comparing sequentially the elements that have the same position in both ranges
against each other until one element is not equivalent to the other.
The result of comparing these first non-matching elements is the result of the lexicographical comparison.
If both sequences compare equal until one of them ends,
the shorter sequence is lexicographically less than the longer one.
*/
template <class InputIt1, class InputIt2>
bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
            InputIt2 first2, InputIt2 last2)
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

//Elements are compared using the given binary comparison function comp.
template <class InputIt1, class InputIt2, class Compare>
bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
            InputIt2 first2, InputIt2 last2, Compare comp)
{
	while (first1 != last1)
	{
		if (first2 == last2 || comp(*first2, *first1)) return false;
		else if (comp(*first1, *first2)) return true;
		++first1;
		++first2;
	}
	return (first2!=last2);
}

    //  pair //
    //---------------------------------------//
template<class T1, class T2>
struct pair {
	typedef T1	first_type;
	typedef T2	second_type;
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