/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 12:07:37 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/17 11:53:55 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstddef>

namespace ft {

//  integral_constant //
//---------------------------------------//
template <class T, T v> struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant<T, v> type;
  operator value_type() { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

// Meta programming helper types //
//---------------------------------------//
template <bool, class T, class U> struct conditional;

template <class T, class U>
struct _or_ : public conditional<T::value, T, U>::type {};
template <class T> struct _or_<T, void> : public T {};
template <class T> struct _or_<void, T> : public T {};
template <> struct _or_<void, void> : public false_type {};

template <class T, class U>
struct _and_ : public conditional<T::value, U, T>::type {};
template <class T> struct _and_<T, void> : public T {};
template <class T> struct _and_<void, T> : public T {};
template <> struct _and_<void, void> : public true_type {};

template <class T>
struct _not_ : public integral_constant<bool, !bool(T::value)> {};

//  is_same //
//---------------------------------------//
// operator bool inherited from integral_constant
//  !=
template <class T, class U> struct is_same : false_type {};
// ==
template <class T> struct is_same<T, T> : true_type {};

//  remove_cv //
//---------------------------------------//
// Remove const/volatile from type
template <class T> struct remove_cv { typedef T type; };
template <class T> struct remove_cv<const T> { typedef T type; };
template <class T> struct remove_cv<volatile T> { typedef T type; };
template <class T> struct remove_cv<const volatile T> { typedef T type; };

//  is_void //
//---------------------------------------//
// is_void_helper
template <class T> struct is_void_helper : false_type {};
template <> struct is_void_helper<void> : true_type {};
// is_void
template <class T>
struct is_void : is_void_helper<typename remove_cv<T>::type>::type {};

//  is_integral //
//---------------------------------------//
// is_integral_helper
// none of the below => NOT integral
template <class T> struct is_integral_helper : false_type {};
// Check if type is integral (including const/volatile qualifications)
//	=> specializations of the template
template <> struct is_integral_helper<bool> : true_type {};
template <> struct is_integral_helper<char> : true_type {};
template <> struct is_integral_helper<wchar_t> : true_type {};
template <> struct is_integral_helper<signed char> : true_type {};
template <> struct is_integral_helper<short int> : true_type {};
template <> struct is_integral_helper<int> : true_type {};
template <> struct is_integral_helper<long int> : true_type {};
template <> struct is_integral_helper<long long int> : true_type {};
template <> struct is_integral_helper<unsigned char> : true_type {};
template <> struct is_integral_helper<unsigned short int> : true_type {};
template <> struct is_integral_helper<unsigned int> : true_type {};
template <> struct is_integral_helper<unsigned long int> : true_type {};
template <> struct is_integral_helper<unsigned long long int> : true_type {};
// is_integral
template <class T>
struct is_integral
    : public is_integral_helper<typename remove_cv<T>::type>::type {};

//  is_floating_point //
//---------------------------------------//
// is_floating_point_helper
template <class T> struct is_floating_point_helper : false_type {};
template <> struct is_floating_point_helper<float> : true_type {};
template <> struct is_floating_point_helper<double> : true_type {};
template <> struct is_floating_point_helper<long double> : true_type {};
// is_floating_point
template <class T>
struct is_floating_point
    : public is_floating_point_helper<typename remove_cv<T>::type>::type {};

//  is_pointer //
//---------------------------------------//
// is_pointer_helper
template <class T> struct is_pointer_helper : false_type {};
template <class T> struct is_pointer_helper<T *> : true_type {};
// is_pointer
template <class T>
struct is_pointer : is_pointer_helper<typename remove_cv<T>::type>::type {};

//  is_member_pointer //
//---------------------------------------//
template <class T> struct is_member_pointer_helper : false_type {};
template <class T, class U>
struct is_member_pointer_helper<T U::*> : true_type {};
template <class T>
struct is_member_pointer
    : is_member_pointer_helper<typename remove_cv<T>::type> {};

//  is_enum //
//---------------------------------------//
template <class T>
struct is_enum : public integral_constant<bool, __is_enum(T)> {};

//  is_union //
//---------------------------------------//
template <class T>
struct is_union : public integral_constant<bool, __is_union(T)> {};

//  is_class //
//---------------------------------------//
template <class T>
struct is_class : public integral_constant<bool, __is_class(T)> {};

//  is_lvalue_ref //
//---------------------------------------//
template <class T> struct is_lvalue_reference : public false_type {};
template <class T> struct is_lvalue_reference<T &> : public true_type {};

//  is_rvalue_ref (C++11) //
//---------------------------------------//
// template <class T> struct is_rvalue_reference : public false_type {};
// template <class T> struct is_rvalue_reference<T &&> : public true_type {};

//  is_reference //
//---------------------------------------//
template <class T> struct is_reference : public is_lvalue_reference<T>::type {};

//  is_arithmetic //
//---------------------------------------//
template <class T>
struct is_arithmetic
    : integral_constant<bool, is_integral<T>::value ||
                                  is_floating_point<T>::value> {};

//  is_fundamemental //
//---------------------------------------//
template <class T>
struct is_fundamental : public _or_<is_arithmetic<T>, is_void<T> >::type {};

//  is_fundamemental //
//---------------------------------------//
template <typename _Tp>
struct is_object : public _not_<_or_<is_reference<_Tp>, is_void<_Tp> > >::type {};

//  is_scalar //
//---------------------------------------//
template <class T>
struct is_scalar
    : integral_constant<bool, is_arithmetic<T>::value || is_pointer<T>::value ||
                                  is_member_pointer<T>::value ||
                                  is_enum<T>::value> {};

//  is_array //
//---------------------------------------//
template <class T> struct is_array : false_type {};
template <class T> struct is_array<T[]> : true_type {};
template <class T, std::size_t N> struct is_array<T[N]> : true_type {};

//  enable_if //
//---------------------------------------//
// Primary template
template <bool Cond, class T = void> struct enable_if {};
// Specialization for TRUE
template <class T> struct enable_if<true, T> { typedef T type; };

//  equal_compare //
//---------------------------------------//
// Test whether the elements in two ranges are equal
// Compares the elements in the range [first1,last1) with those in the range
// beginning at first2, and returns true if all of the elements in both ranges
// match.
template <class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
  while (first1 != last1) {
    if (!(*first1 == *first2))
      return false;
    ++first1;
    ++first2;
  }
  return true;
}

/*
binary predicate which returns ​true if the elements should be treated as
equal. The signature of the predicate function should be equivalent to the
following:

 bool pred(const Type1 &a, const Type2 &b);

While the signature does not need to have const &,
the function must not modify the objects passed to it and must be able to accept
all values of type (possibly const) Type1 and Type2 regardless of value category
(thus, Type1 & is not allowed).
*/
template <class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2,
           BinaryPredicate p) {
  while (first1 != last1) {
    if (!p(*first1, *first2))
      return false;
    ++first1;
    ++first2;
  }
  return true;
}

//  lexographical_compare //
//---------------------------------------//
/*
Returns true if the range [first1,last1) compares lexicographically less than
the range [first2,last2). A lexicographical comparison is the kind of comparison
-> generally used to sort words alphabetically in dictionaries;
It involves comparing sequentially the elements that have the same position in
both ranges against each other until one element is not equivalent to the other.
The result of comparing these first non-matching elements is the result of the
lexicographical comparison. If both sequences compare equal until one of them
ends, the shorter sequence is lexicographically less than the longer one.
*/
template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2) {
  while (first1 != last1) {
    if (first2 == last2 || *first2 < *first1)
      return false;
    else if (*first1 < *first2)
      return true;
    ++first1;
    ++first2;
  }
  return (first2 != last2);
}

// Elements are compared using the given binary comparison function comp.
template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, Compare comp) {
  while (first1 != last1) {
    if (first2 == last2 || comp(*first2, *first1))
      return false;
    else if (comp(*first1, *first2))
      return true;
    ++first1;
    ++first2;
  }
  return (first2 != last2);
}

//  pair //
//---------------------------------------//
template <class T1, class T2> struct pair {
  typedef T1 first_type;
  typedef T2 second_type;
  first_type first;
  second_type second;

  pair() : first(), second(){};
  template <class T, class U>
  pair(pair<T, U> const &src) : first(src.first), second(src.second){};
  pair(first_type const &a, second_type const &b) : first(a), second(b){};

  pair &operator=(pair const &rhs) {
    if (*this != rhs) {
      first = rhs.first;
      second = rhs.second;
    }
    return *this;
  }
  //  pair - relational operators //
  friend bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
  }
  friend bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return !(lhs == rhs);
  }
  friend bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return lhs.first < rhs.first ||
           (!(rhs.first < lhs.first) && lhs.second < rhs.second);
  }
  friend bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return !(rhs < lhs);
  }
  friend bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return rhs < lhs;
  }
  friend bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
    return !(lhs < rhs);
  }
};

//  pair - make_pair //
template <class T1, class T2> pair<T1, T2> make_pair(T1 x, T2 y) {
  return (pair<T1, T2>(x, y));
}

} // namespace ft
#endif