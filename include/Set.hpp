/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Set.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 17:00:26 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/29 10:32:41 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SET_HPP
#define SET_HPP

#include "Iterators.hpp"
#include "RedBlackTree.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <stdexcept>

namespace ft {
template <typename Key, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<Key> >
class set {
public:
  typedef Key key_type;
  typedef Key value_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
  typedef Alloc allocator_type;

private:
  typedef RedBlackTree<key_type, value_type, std::_Identity<value_type>,
                       key_compare, Alloc>
      tree_type;
  tree_type tree; // Red-black tree representing set.
public:
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::const_pointer const_pointer;
  typedef typename Alloc::reference reference;
  typedef typename Alloc::const_reference const_reference;
  typedef typename tree_type::const_iterator iterator;
  typedef typename tree_type::const_iterator const_iterator;
  typedef typename tree_type::const_reverse_iterator reverse_iterator;
  typedef typename tree_type::const_reverse_iterator const_reverse_iterator;
  typedef typename tree_type::size_type size_type;
  typedef typename tree_type::difference_type difference_type;
  // allocation/deallocation
  ///
  /// @brief  Default constructor creates no elements.
  ///
  set() : tree() {}
  ///
  /// @brief  Creates a set with no elements.
  /// @param  comp  Comparator to use.
  /// @param  a  An allocator object.
  ///
  explicit set(const Compare &comp, const allocator_type &a = allocator_type())
      : tree(comp, Alloc(a)) {}
  ///
  /// @brief  Builds a set from a range.
  /// @param  first  An input iterator.
  /// @param  last  An input iterator.
  ///
  /// Create a set consisting of copies of the elements from
  /// [first,last).  This is linear in N if the range is
  /// already sorted, and NlogN otherwise (where N is
  /// distance(first,last)).
  ///
  template <typename InputIterator>
  set(InputIterator first, InputIterator last) : tree() {
    tree.insert_unique(first, last);
  }
  ///
  /// @brief  Builds a set from a range.
  /// @param  first  An input iterator.
  /// @param  last  An input iterator.
  /// @param  comp  A comparison functor.
  /// @param  a  An allocator object.
  ///
  /// Create a set consisting of copies of the elements from
  /// [first,last).  This is linear in N if the range is
  /// already sorted, and NlogN otherwise (where N is
  /// distance(first,last)).
  ///
  template <typename InputIterator>
  set(InputIterator first, InputIterator last, const Compare &comp,
      const allocator_type &a = allocator_type())
      : tree(comp, Alloc(a)) {
    tree.insert_unique(first, last);
  }
  ///
  /// @brief  Set copy constructor.
  ///
  /// Whether the allocator is copied depends on the allocator traits.
  ///
  set(const set &x) : tree(x.tree) {}
  ///
  /// @brief  Set assignment operator.
  ///
  /// Whether the allocator is copied depends on the allocator traits.
  ///
  set &operator=(const set &x) {
    tree = x.tree;
    return *this;
  }
  // accessors:
  ///  Returns the comparison object with which the set was constructed.
  key_compare key_comp() const { return tree.key_comp(); }
  ///  Returns the comparison object with which the set was constructed.
  value_compare value_comp() const { return tree.key_comp(); }
  ///  Returns the allocator object with which the set was constructed.
  allocator_type get_allocator() const {
    return allocator_type(tree.get_allocator());
  }
  ///
  /// Returns a read-only (constant) iterator that points to the first
  /// element in the set.  Iteration is done in ascending order according
  /// to the keys.
  ///
  iterator begin() const { return tree.begin(); }
  ///
  /// Returns a read-only (constant) iterator that points one past the last
  /// element in the set.  Iteration is done in ascending order according
  /// to the keys.
  ///
  iterator end() const { return tree.end(); }
  ///
  /// Returns a read-only (constant) iterator that points to the last
  /// element in the set.  Iteration is done in descending order according
  /// to the keys.
  ///
  reverse_iterator rbegin() const { return tree.rbegin(); }
  ///
  /// Returns a read-only (constant) reverse iterator that points to the
  /// last pair in the set.  Iteration is done in descending order
  /// according to the keys.
  ///
  reverse_iterator rend() const { return tree.rend(); }
  ///  Returns true if the set is empty.
  bool empty() const { return tree.empty(); }
  ///  Returns the size of the set.
  size_type size() const { return tree.size(); }
  ///  Returns the maximum size of the set.
  size_type max_size() const { return tree.max_size(); }
  ///
  /// @brief  Swaps data with another set.
  /// @param  x  A set of the same element and allocator types.
  ///
  /// This exchanges the elements between two sets in constant
  /// time.  (It is only swapping a pointer, an integer, and an
  /// instance of the @c Compare type (which itself is often
  /// stateless and empty), so it should be quite fast.)  Note
  /// that the global std::swap() function is specialized such
  /// that std::swap(s1,s2) will feed to this function.
  ///
  /// Whether the allocators are swapped depends on the allocator traits.
  ///
  void swap(set &x) { tree.swap(x.tree); }
  // insert/erase
  ///
  /// @brief Attempts to insert an element into the set.
  /// @param  x  Element to be inserted.
  /// @return  A pair, of which the first element is an iterator that points
  ///          to the possibly inserted element, and the second is a bool
  ///          that is true if the element was actually inserted.
  ///
  /// This function attempts to insert an element into the set.  A set
  /// relies on unique keys and thus an element is only inserted if it is
  /// not already present in the set.
  ///
  /// Insertion requires logarithmic time.
  ///
  ft::pair<iterator, bool> insert(const value_type &x) {
    ft::pair<typename tree_type::iterator, bool> p = tree.insert_unique(x);
    return ft::pair<iterator, bool>(p.first, p.second);
  }
  ///
  /// @brief Attempts to insert an element into the set.
  /// @param  position  An iterator that serves as a hint as to where the
  ///                   element should be inserted.
  /// @param  x  Element to be inserted.
  /// @return An iterator that points to the element with key of
  ///          @a x (may or may not be the element passed in).
  ///
  /// This function is not concerned about whether the insertion took place,
  /// and thus does not return a boolean like the single-argument insert()
  /// does.  Note that the first parameter is only a hint and can
  /// potentially improve the performance of the insertion process.  A bad
  /// hint would cause no gains in efficiency.
  ///
  /// For more on @a hinting, see:
  /// https://gcc.gnu.org/onlinedocs/libstdc++/manual/associative.html#containers.associative.insert_hints
  ///
  /// Insertion requires logarithmic time (if the hint is not taken).
  ///
  iterator insert(const_iterator position, const value_type &x) {
    return tree.insert_unique(position, x);
  }
  ///
  /// @brief A template function that attempts to insert a range
  /// of elements.
  /// @param  first  Iterator pointing to the start of the range to be
  ///                  inserted.
  /// @param  last  Iterator pointing to the end of the range.
  ///
  /// Complexity similar to that of the range constructor.
  ///
  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last) {
    tree.insert_unique(first, last);
  }
  ///
  /// @brief Erases an element from a set.
  /// @param  position  An iterator pointing to the element to be erased.
  ///
  /// This function erases an element, pointed to by the given iterator,
  /// from a set.  Note that this function only erases the element, and
  /// that if the element is itself a pointer, the pointed-to memory is not
  /// touched in any way.  Managing the pointer is the user's
  /// responsibility.
  ///
  void erase(iterator position) { tree.erase(position); }
  ///
  /// @brief Erases elements according to the provided key.
  /// @param  x  Key of element to be erased.
  /// @return  The number of elements erased.
  ///
  /// This function erases all the elements located by the given key from
  /// a set.
  /// Note that this function only erases the element, and that if
  /// the element is itself a pointer, the pointed-to memory is not touched
  /// in any way.  Managing the pointer is the user's responsibility.
  ///
  size_type erase(const key_type &x) { return tree.erase(x); }
  ///
  /// @brief Erases a [first,last) range of elements from a set.
  /// @param  first  Iterator pointing to the start of the range to be
  ///                erased.
  /// @param last Iterator pointing to the end of the range to
  /// be erased.
  ///
  /// This function erases a sequence of elements from a set.
  /// Note that this function only erases the element, and that if
  /// the element is itself a pointer, the pointed-to memory is not touched
  /// in any way.  Managing the pointer is the user's responsibility.
  ///
  void erase(iterator first, iterator last) { tree.erase(first, last); }
  ///
  /// Erases all elements in a set.  Note that this function only erases
  /// the elements, and that if the elements themselves are pointers, the
  /// pointed-to memory is not touched in any way.  Managing the pointer is
  /// the user's responsibility.
  ///
  void clear() { tree.clear(); }
  // set operations:

  ///
  /// @brief  Finds the number of elements.
  /// @param  x  Element to located.
  /// @return  Number of elements with specified key.
  ///
  /// This function only makes sense for multisets; for set the result will
  /// either be 0 (not present) or 1 (present).
  ///
  size_type count(const key_type &x) const {
    return tree.find(x) == tree.end() ? 0 : 1;
  }

  ///
  /// @brief Tries to locate an element in a set.
  /// @param  x  Element to be located.
  /// @return  Iterator pointing to sought-after element, or end() if not
  ///          found.
  ///
  /// This function takes a key and tries to locate the element with which
  /// the key matches.  If successful the function returns an iterator
  /// pointing to the sought after element.  If unsuccessful it returns the
  /// past-the-end ( @c end() ) iterator.
  ///
  iterator find(const key_type &x) { return tree.find(x); }
  const_iterator find(const key_type &x) const { return tree.find(x); }

  ///
  /// @brief Finds the beginning of a subsequence matching given key.
  /// @param  x  Key to be located.
  /// @return  Iterator pointing to first element equal to or greater
  ///          than key, or end().
  ///
  /// This function returns the first element of a subsequence of elements
  /// that matches the given key.  If unsuccessful it returns an iterator
  /// pointing to the first element that has a greater value than given key
  /// or end() if no such element exists.
  ///
  iterator lower_bound(const key_type &x) { return tree.lower_bound(x); }
  const_iterator lower_bound(const key_type &x) const {
    return tree.lower_bound(x);
  }

  ///
  /// @brief Finds the end of a subsequence matching given key.
  /// @param  x  Key to be located.
  /// @return Iterator pointing to the first element
  ///         greater than key, or end().
  ///
  iterator upper_bound(const key_type &x) { return tree.upper_bound(x); }
  const_iterator upper_bound(const key_type &x) const {
    return tree.upper_bound(x);
  }

  ///
  /// @brief Finds a subsequence matching given key.
  /// @param  x  Key to be located.
  /// @return  Pair of iterators that possibly points to the subsequence
  ///          matching given key.
  ///
  /// This function is equivalent to
  /// @code
  ///   std::make_pair(c.lower_bound(val),
  ///                  c.upper_bound(val))
  /// @endcode
  /// (but is faster than making the calls separately).
  ///
  /// This function probably only makes sense for multisets.
  ///
  ft::pair<iterator, iterator> equal_range(const key_type &x) {
    return tree.equal_range(x);
  }
  ft::pair<const_iterator, const_iterator>
  equal_range(const key_type &x) const {
    return tree.equal_range(x);
  }

  template <typename K1, typename C1, typename A1>
  friend bool operator==(const set<K1, C1, A1> &, const set<K1, C1, A1> &);
  template <typename K1, typename C1, typename A1>
  friend bool operator<(const set<K1, C1, A1> &, const set<K1, C1, A1> &);
};
///
/// @brief  Set equality comparison.
/// @param  x  A set.
/// @param  y  A set of the same type as @a x.
/// @return  True iff the size and elements of the sets are equal.
///
/// This is an equivalence relation.  It is linear in the size of the sets.
/// Sets are considered equivalent if their sizes are equal, and if
/// corresponding elements compare equal.
///
template <typename Key, typename Compare, typename Alloc>
inline bool operator==(const set<Key, Compare, Alloc> &x,
                       const set<Key, Compare, Alloc> &y) {
  return x.tree == y.tree;
}
///
/// @brief  Set ordering relation.
/// @param  x  A set.
/// @param  y  A set of the same type as @a x.
/// @return  True iff @a x is lexicographically less than @a y.
///
/// This is a total ordering relation.  It is linear in the size of the
/// sets.  The elements must be comparable with @c <.
///
/// See std::lexicographicalcompare() for how the determination is made.
///
template <typename Key, typename Compare, typename Alloc>
inline bool operator<(const set<Key, Compare, Alloc> &x,
                      const set<Key, Compare, Alloc> &y) {
  return x.tree < y.tree;
}
///  Returns !(x == y).
template <typename Key, typename Compare, typename Alloc>
inline bool operator!=(const set<Key, Compare, Alloc> &x,
                       const set<Key, Compare, Alloc> &y) {
  return !(x == y);
}
///  Returns y < x.
template <typename Key, typename Compare, typename Alloc>
inline bool operator>(const set<Key, Compare, Alloc> &x,
                      const set<Key, Compare, Alloc> &y) {
  return y < x;
}
///  Returns !(y < x)
template <typename Key, typename Compare, typename Alloc>
inline bool operator<=(const set<Key, Compare, Alloc> &x,
                       const set<Key, Compare, Alloc> &y) {
  return !(y < x);
}
///  Returns !(x < y)
template <typename Key, typename Compare, typename Alloc>
inline bool operator>=(const set<Key, Compare, Alloc> &x,
                       const set<Key, Compare, Alloc> &y) {
  return !(x < y);
}
/// See std::set::swap().
template <typename Key, typename Compare, typename Alloc>
inline void swap(set<Key, Compare, Alloc> &x, set<Key, Compare, Alloc> &y) {
  x.swap(y);
}

} // namespace ft

#endif