/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:54 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/28 11:18:08 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
#define MAP_HPP

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

template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<ft::pair<const Key, T> > >
class map {
public:
  typedef Key key_type;
  typedef T mapped_type;
  typedef ft::pair<const Key, T> value_type;
  typedef Compare key_compare;
  typedef Alloc allocator_type;

public:
  class value_compare
      : public std::binary_function<value_type, value_type, bool> {
    friend class map<Key, T, Compare, Alloc>;

  protected:
    Compare comp;
    value_compare(Compare c) : comp(c) {}

  public:
    bool operator()(const value_type &x, const value_type &y) const {
      return comp(x.first, y.first);
    }
  };

private:
  /// This turns a red-black tree into a map.
  typedef ft::RedBlackTree<key_type, value_type, std::_Select1st<value_type>,
                           key_compare, allocator_type>
      tree_type;
  /// The actual tree structure.
  tree_type tree;

public:
  // many of these are specified differently in ISO, but the following are
  // "functionally equivalent"
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::const_pointer const_pointer;
  typedef typename Alloc::reference reference;
  typedef typename Alloc::const_reference const_reference;
  typedef typename tree_type::iterator iterator;
  typedef typename tree_type::const_iterator const_iterator;
  typedef typename tree_type::size_type size_type;
  typedef typename tree_type::difference_type difference_type;
  typedef typename tree_type::reverse_iterator reverse_iterator;
  typedef typename tree_type::const_reverse_iterator const_reverse_iterator;

  map() : tree() {}
  explicit map(const Compare &comp, const allocator_type &a = allocator_type())
      : tree(comp, allocator_type(a)) {}
  map(const map &x) : tree(x.tree) {}
  /// 
  /// @brief  Builds a map from a range.
  /// @param  first  An input iterator.
  /// @param  last  An input iterator.
  ///
  /// Create a map consisting of copies of the elements from
  /// [first,last).  This is linear in N if the range is
  /// already sorted, and NlogN otherwise (where N is
  /// distance(first,last)).
  ///
  template <typename InputIterator>
  map(InputIterator first, InputIterator last) : tree() {
    tree.insert_unique(first, last);
  }
  /// 
  /// @brief  Builds a map from a range.
  /// @param  first  An input iterator.
  /// @param  last  An input iterator.
  /// @param  comp  A comparison functor.
  /// @param  a  An allocator object.
  ///
  /// Create a map consisting of copies of the elements from
  /// [first,last).  This is linear in N if the range is
  /// already sorted, and NlogN otherwise (where N is
  /// distance(first,last)).
  ///
  template <typename InputIterator>
  map(InputIterator first, InputIterator last, const Compare &comp,
      const allocator_type &a = allocator_type())
      : tree(comp, allocator_type(a)) {
    tree.insert_unique(first, last);
  }

  ~map() {
  }

  /// 
  /// @brief  Map assignment operator.
  ///
  /// Whether the allocator is copied depends on the allocator traits.
  ///
  map &operator=(const map &x) {
    tree = x.tree;
    return *this;
  }
  /// Get a copy of the memory allocation object.
  allocator_type get_allocator() const {
    return allocator_type(tree.get_allocator());
  }

  // iterators

  /// 
  /// Returns a read/write iterator that points to the first pair in the
  /// map.
  /// Iteration is done in ascending order according to the keys.
  ///
  iterator begin() { return tree.begin(); }
  /// 
  /// Returns a read-only (constant) iterator that points to the first pair
  /// in the map.  Iteration is done in ascending order according to the
  /// keys.
  ///
  const_iterator begin() const { return tree.begin(); }
  /// 
  /// Returns a read/write iterator that points one past the last
  /// pair in the map.  Iteration is done in ascending order
  /// according to the keys.
  ///
  iterator end() { return tree.end(); }
  /// 
  /// Returns a read-only (constant) iterator that points one past the last
  /// pair in the map.  Iteration is done in ascending order according to
  /// the keys.
  ///
  const_iterator end() const { return tree.end(); }
  /// 
  /// Returns a read/write reverse iterator that points to the last pair in
  /// the map.  Iteration is done in descending order according to the
  /// keys.
  ///
  reverse_iterator rbegin() { return tree.rbegin(); }
  /// 
  /// Returns a read-only (constant) reverse iterator that points to the
  /// last pair in the map.  Iteration is done in descending order
  /// according to the keys.
  ///
  const_reverse_iterator rbegin() const { return tree.rbegin(); }
  /// 
  /// Returns a read/write reverse iterator that points to one before the
  /// first pair in the map.  Iteration is done in descending order
  /// according to the keys.
  ///
  reverse_iterator rend() { return tree.rend(); }
  /// 
  /// Returns a read-only (constant) reverse iterator that points to one
  /// before the first pair in the map.  Iteration is done in descending
  /// order according to the keys.
  ///
  const_reverse_iterator rend() const { return tree.rend(); }

  // capacity

  /// Returns true if the map is empty.  (Thus begin() would equal
  /// end().)
  ///
  bool empty() const { return tree.empty(); }
   /// Returns the size of the map. ///
  size_type size() const { return tree.size(); }
   /// Returns the maximum size of the map. ///
  size_type max_size() const { return tree.max_size(); }

  // element access

  /// 
  /// @brief  Subscript ( @c [] ) access to map data.
  /// @param  k  The key for which data should be retrieved.
  /// @return  A reference to the data of the (key,data) pair.
  ///
  /// Allows for easy lookup with the subscript ( @c [] )
  /// operator.  Returns data associated with the key specified in
  /// subscript.  If the key does not exist, a pair with that key
  /// is created using default values, which is then returned.
  ///
  /// Lookup requires logarithmic time.
  ///
  mapped_type &operator[](const key_type &k) {
    iterator i = lower_bound(k);
    // i->first is greater than or equivalent to k.
    if (i == end() || key_comp()(k, (*i).first))
      i = insert(i, value_type(k, mapped_type()));
    return (*i).second;
  }
  /// 
  /// @brief  Access to map data.
  /// @param  k  The key for which data should be retrieved.
  /// @return  A reference to the data whose key is equivalent to @a k, if
  ///          such a data is present in the map.
  /// @throw  std::out_of_range  If no such data is present.
  ///
  mapped_type &at(const key_type &k) {
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first))
      std::__throw_out_of_range(__N("map::at"));
    return (*i).second;
  }
  const mapped_type &at(const key_type &k) const {
    const_iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first))
      std::__throw_out_of_range(__N("map::at"));
    return (*i).second;
  }

  /// 
  /// @brief Attempts to insert a ft::pair into the map.
  /// @param x Pair to be inserted (see ft::make_pair for easy
  ///            creation of pairs).
  ///
  /// @return  A pair, of which the first element is an iterator that
  ///          points to the possibly inserted pair, and the second is
  ///          a bool that is true if the pair was actually inserted.
  ///
  /// This function attempts to insert a (key, value) pair into the map.
  /// A map relies on unique keys and thus a pair is only inserted if its
  /// first element (the key) is not already present in the map.
  ///
  /// Insertion requires logarithmic time.
  ///
  ft::pair<iterator, bool> insert(const value_type &x) {
    return tree.insert_unique(x);
  }

  /// 
  /// @brief Attempts to insert a ft::pair into the map.
  /// @param  position  An iterator that serves as a hint as to where the
  ///                   pair should be inserted.
  /// @param  x  Pair to be inserted (see ft::make_pair for easy creation
  ///              of pairs).
  /// @return An iterator that points to the element with key of
  ///          @a x (may or may not be the pair passed in).
  ///
  /// This function is not concerned about whether the insertion
  /// took place, and thus does not return a boolean like the
  /// single-argument insert() does.  Note that the first
  /// parameter is only a hint and can potentially improve the
  /// performance of the insertion process.  A bad hint would
  /// cause no gains in efficiency.
  ///
  /// Insertion requires logarithmic time (if the hint is not taken).
  ///
  iterator insert(iterator position, const value_type &x) {
    return tree.insert_unique(position, x);
  }

  /// 
  /// @brief Template function that attempts to insert a range of elements.
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
  /// @brief Erases an element from a map.
  /// @param  position  An iterator pointing to the element to be erased.
  ///
  /// This function erases an element, pointed to by the given
  /// iterator, from a map.  Note that this function only erases
  /// the element, and that if the element is itself a pointer,
  /// the pointed-to memory is not touched in any way.  Managing
  /// the pointer is the user's responsibility.
  ///
  void erase(iterator position) { tree.erase(position); }

  /// 
  /// @brief Erases elements according to the provided key.
  /// @param  x  Key of element to be erased.
  /// @return  The number of elements erased.
  ///
  /// This function erases all the elements located by the given key from
  /// a map.
  /// Note that this function only erases the element, and that if
  /// the element is itself a pointer, the pointed-to memory is not touched
  /// in any way.  Managing the pointer is the user's responsibility.
  ///
  size_type erase(const key_type &x) { return tree.erase(x); }

  /// 
  /// @brief Erases a [first,last) range of elements from a map.
  /// @param  first  Iterator pointing to the start of the range to be
  ///                  erased.
  /// @param last Iterator pointing to the end of the range to
  ///               be erased.
  ///
  /// This function erases a sequence of elements from a map.
  /// Note that this function only erases the element, and that if
  /// the element is itself a pointer, the pointed-to memory is not touched
  /// in any way.  Managing the pointer is the user's responsibility.
  ///
  void erase(iterator first, iterator last) { tree.erase(first, last); }

  /// 
  /// @brief  Swaps data with another map.
  /// @param  x  A map of the same element and allocator types.
  ///
  /// This exchanges the elements between two maps in constant
  /// time.  (It is only swapping a pointer, an integer, and an
  /// instance of the @c Compare type (which itself is often
  /// stateless and empty), so it should be quite fast.)  Note
  /// that the global std::swap() function is specialized such
  /// that std::swap(m1,m2) will feed to this function.
  ///
  /// Whether the allocators are swapped depends on the allocator traits.
  ///
  void swap(map &x) { tree.swap(x.tree); }
  /// 
  /// Erases all elements in a map.  Note that this function only
  /// erases the elements, and that if the elements themselves are
  /// pointers, the pointed-to memory is not touched in any way.
  /// Managing the pointer is the user's responsibility.
  ///
  void clear() { tree.clear(); }

  // observers

  /// 
  /// Returns the key comparison object out of which the map was
  /// constructed.
  ///
  key_compare key_comp() const { return tree.key_comp(); }
  /// 
  /// Returns a value comparison object, built from the key comparison
  /// object out of which the map was constructed.
  ///
  value_compare value_comp() const { return value_compare(tree.key_comp()); }

  // map operations

  /// 
  /// @brief Tries to locate an element in a map.
  /// @param  x  Key of (key, value) pair to be located.
  /// @return  Iterator pointing to sought-after element, or end() if not
  ///          found.
  ///
  /// This function takes a key and tries to locate the element with which
  /// the key matches.  If successful the function returns an iterator
  /// pointing to the sought after pair.  If unsuccessful it returns the
  /// past-the-end ( @c end() ) iterator.
  ///
  iterator find(const key_type &x) { return tree.find(x); }

  /// 
  /// @brief Tries to locate an element in a map.
  /// @param  x  Key of (key, value) pair to be located.
  /// @return  Read-only (constant) iterator pointing to sought-after
  ///          element, or end() if not found.
  ///
  /// This function takes a key and tries to locate the element with which
  /// the key matches.  If successful the function returns a constant
  /// iterator pointing to the sought after pair. If unsuccessful it
  /// returns the past-the-end ( @c end() ) iterator.
  ///
  const_iterator find(const key_type &x) const { return tree.find(x); }

  /// 
  /// @brief  Finds the number of elements with given key.
  /// @param  x  Key of (key, value) pairs to be located.
  /// @return  Number of elements with specified key.
  ///
  /// This function only makes sense for multimaps; for map the result will
  /// either be 0 (not present) or 1 (present).
  ///
  size_type count(const key_type &x) const {
    return tree.find(x) == tree.end() ? 0 : 1;
  }

  /// 
  /// @brief Finds the beginning of a subsequence matching given key.
  /// @param  x  Key of (key, value) pair to be located.
  /// @return  Iterator pointing to first element equal to or greater
  ///          than key, or end().
  ///
  /// This function returns the first element of a subsequence of elements
  /// that matches the given key.  If unsuccessful it returns an iterator
  /// pointing to the first element that has a greater value than given key
  /// or end() if no such element exists.
  ///
  iterator lower_bound(const key_type &x) { return tree.lower_bound(x); }

  /// 
  /// @brief Finds the beginning of a subsequence matching given key.
  /// @param  x  Key of (key, value) pair to be located.
  /// @return  Read-only (constant) iterator pointing to first element
  ///          equal to or greater than key, or end().
  ///
  /// This function returns the first element of a subsequence of elements
  /// that matches the given key.  If unsuccessful it returns an iterator
  /// pointing to the first element that has a greater value than given key
  /// or end() if no such element exists.
  ///
  const_iterator lower_bound(const key_type &x) const {
    return tree.lower_bound(x);
  }

  /// 
  /// @brief Finds the end of a subsequence matching given key.
  /// @param  x  Key of (key, value) pair to be located.
  /// @return Iterator pointing to the first element
  ///         greater than key, or end().
  ///
  iterator upper_bound(const key_type &x) { return tree.upper_bound(x); }

  /// 
  /// @brief Finds the end of a subsequence matching given key.
  /// @param  x  Key of (key, value) pair to be located.
  /// @return  Read-only (constant) iterator pointing to first iterator
  ///          greater than key, or end().
  ///
  const_iterator upper_bound(const key_type &x) const {
    return tree.upper_bound(x);
  }

  /// 
  /// @brief Finds a subsequence matching given key.
  /// @param  x  Key of (key, value) pairs to be located.
  /// @return  Pair of iterators that possibly points to the subsequence
  ///          matching given key.
  ///
  /// This function is equivalent to
  /// @code
  ///   ft::make_pair(c.lower_bound(val),
  ///                  c.upper_bound(val))
  /// @endcode
  /// (but is faster than making the calls separately).
  ///
  /// This function probably only makes sense for multimaps.
  ///
  ft::pair<iterator, iterator> equal_range(const key_type &x) {
    return tree.equal_range(x);
  }

  /// 
  /// @brief Finds a subsequence matching given key.
  /// @param  x  Key of (key, value) pairs to be located.
  /// @return  Pair of read-only (constant) iterators that possibly points
  ///          to the subsequence matching given key.
  ///
  /// This function is equivalent to
  /// @code
  ///   ft::make_pair(c.lower_bound(val),
  ///                  c.upper_bound(val))
  /// @endcode
  /// (but is faster than making the calls separately).
  ///
  /// This function probably only makes sense for multimaps.
  ///
  ft::pair<const_iterator, const_iterator>
  equal_range(const key_type &x) const {
    return tree.equal_range(x);
  }
};

///
/// @brief  Map equality comparison.
/// @param  x  A map.
/// @param  y  A map of the same type as @a x.
/// @return  True iff the size and elements of the maps are equal.
///
/// This is an equivalence relation.  It is linear in the size of the
/// maps.  Maps are considered equivalent if their sizes are equal,
/// and if corresponding elements compare equal.
///
template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator==(const map<Key, T, Compare, Alloc> &x,
                       const map<Key, T, Compare, Alloc> &y) {
  return x.tree == y.tree;
}
///
/// @brief  Map ordering relation.
/// @param  x  A map.
/// @param  y  A map of the same type as @a x.
/// @return  True iff @a x is lexicographically less than @a y.
///
/// This is a total ordering relation.  It is linear in the size of the
/// maps.  The elements must be comparable with @c <.
///
/// See std::lexicographical_compare() for how the determination is made.
///
template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator<(const map<Key, T, Compare, Alloc> &x,
                      const map<Key, T, Compare, Alloc> &y) {
  return x.tree < y.tree;
}
/// Based on operator==
template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator!=(const map<Key, T, Compare, Alloc> &x,
                       const map<Key, T, Compare, Alloc> &y) {
  return !(x == y);
}
/// Based on operator<
template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator>(const map<Key, T, Compare, Alloc> &x,
                      const map<Key, T, Compare, Alloc> &y) {
  return y < x;
}
/// Based on operator<
template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator<=(const map<Key, T, Compare, Alloc> &x,
                       const map<Key, T, Compare, Alloc> &y) {
  return !(y < x);
}
/// Based on operator<
template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator>=(const map<Key, T, Compare, Alloc> &x,
                       const map<Key, T, Compare, Alloc> &y) {
  return !(x < y);
}
/// See std::map::swap().
template <typename Key, typename T, typename Compare, typename Alloc>
inline void swap(map<Key, T, Compare, Alloc> &x,
                 map<Key, T, Compare, Alloc> &y) {
  x.swap(y);
}

} // namespace ft

#endif