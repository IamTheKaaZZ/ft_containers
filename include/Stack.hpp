/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:45:30 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/28 16:48:12 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
#define STACK_HPP

#include "Vector.hpp"

namespace ft {
template <typename T, typename Sequence = ft::vector<T> > class stack {

  template <typename T1, typename Seq1>
  friend bool operator==(const stack<T1, Seq1> &, const stack<T1, Seq1> &);
  template <typename T1, typename Seq1>
  friend bool operator<(const stack<T1, Seq1> &, const stack<T1, Seq1> &);

public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::reference reference;
  typedef typename Sequence::const_reference const_reference;
  typedef typename Sequence::size_type size_type;
  typedef Sequence container_type;

protected:
  Sequence c;
  ///
  /// @brief Check if the container is not empty.
  ///
  ///
  void requireNonEmpty() {
    if (empty())
      throw ft::ContainerIsEmptyError();
  }

public:
  ///
  /// @brief  Default constructor creates no elements.
  /// 
  explicit stack(const Sequence &c = Sequence()) : c(c) {}
  ///
  /// Returns true if the stack is empty.
  /// 
  bool empty() const { return c.empty(); }
  ///  Returns the number of elements in the stack.   /// 
  size_type size() const { return c.size(); }
  ///
  /// Returns a read/write reference to the data at the first
  /// element of the stack.
  /// 
  reference top() {
    requireNonEmpty();
    return c.back();
  }
  ///
  /// Returns a read-only (constant) reference to the data at the first
  /// element of the stack.
  /// 
  const_reference top() const {
    requireNonEmpty();
    return c.back();
  }
  ///
  /// @brief  Add data to the top of the stack.
  /// @param  x  Data to be added.
  ///
  /// This is a typical stack operation.  The function creates an
  /// element at the top of the stack and assigns the given data
  /// to it.  The time complexity of the operation depends on the
  /// underlying sequence.
  /// 
  void push(const value_type &x) { c.push_back(x); }
  ///
  /// @brief  Removes first element.
  ///
  /// This is a typical stack operation.  It shrinks the stack
  /// by one.  The time complexity of the operation depends on the
  /// underlying sequence.
  ///
  /// Note that no data is returned, and if the first element's
  /// data is needed, it should be retrieved before pop() is
  /// called.
  /// 
  void pop() {
    requireNonEmpty();
    c.pop_back();
  }
};
///
/// @brief  Stack equality comparison.
/// @param  x  A stack.
/// @param  y  A stack of the same type as @a x.
/// @return  True if the size and elements of the stacks are equal.
 ///
/// This is an equivalence relation.  Complexity and semantics
/// depend on the underlying sequence type, but the expected rules
/// are: this relation is linear in the size of the sequences, and
/// stacks are considered equivalent if their sequences compare
/// equal.
  ///
template <typename T, typename Seq>
inline bool operator==(const stack<T, Seq> &x, const stack<T, Seq> &y) {
  return x.c == y.c;
}
///
/// @brief  Stack ordering relation.
/// @param  x  A stack.
/// @param  y  A stack of the same type as @a x.
/// @return  True if @a x is lexicographically less than @a y.
 ///
/// This is an total ordering relation.  Complexity and semantics
/// depend on the underlying sequence type, but the expected rules
/// are: this relation is linear in the size of the sequences, the
/// elements must be comparable with @c <, and
/// std::lexicographical_compare() is usually used to make the
/// determination.
  ///
template <typename T, typename Seq>
inline bool operator<(const stack<T, Seq> &x, const stack<T, Seq> &y) {
  return x.c < y.c;
}
/// Based on operator==
template <typename T, typename Seq>
inline bool operator!=(const stack<T, Seq> &x, const stack<T, Seq> &y) {
  return !(x == y);
}
/// Based on operator<
template <typename T, typename Seq>
inline bool operator>(const stack<T, Seq> &x, const stack<T, Seq> &y) {
  return y < x;
}
/// Based on operator<
template <typename T, typename Seq>
inline bool operator<=(const stack<T, Seq> &x, const stack<T, Seq> &y) {
  return !(y < x);
}
/// Based on operator<
template <typename T, typename Seq>
inline bool operator>=(const stack<T, Seq> &x, const stack<T, Seq> &y) {
  return !(x < y);
}

} // namespace ft

#endif