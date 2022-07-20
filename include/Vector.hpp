/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/20 15:40:40 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Iterators.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <exception>
#include <iterator>
#include <list>
#include <memory>
#include <stdexcept>
#include <iostream>
namespace ft {

class ContainerIsEmptyError : public std::exception {
    virtual const char* what() const throw() {
        return "The container is empty, this can cause undefined beahviour.";
    }
};

template <class T, class Allocator = std::allocator<T> > struct vectorBase {
  // Implicit vector data
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::size_type size_type;

public:
  allocator_type get_allocator() const { return alloc; }
  vectorBase() : alloc(), start(), finish(), endOfStorage() {}
  vectorBase(allocator_type const &alloc)
      : alloc(alloc), start(), finish(), endOfStorage() {}
  vectorBase(size_type n, allocator_type const &alloc)
      : alloc(alloc), start(), finish(), endOfStorage() {
    createStorage(n);
  }
  ~vectorBase() {
    destroyAll();
    deallocate(start, capacity());
  }

  void copyData(const vectorBase &rhs) {
    destroyAll();
    deallocate(start, capacity());
    alloc = rhs.alloc;
    start = pointer();
    finish = pointer();
    endOfStorage = pointer();
    createStorage(rhs.size());
    finish = construct(start, rhs.start, rhs.finish);
  }
  void swapData(vectorBase &rhs) {
    vectorBase tmp;
    tmp.copyData(*this);
    copyData(rhs);
    rhs.copyData(tmp);
  }

  pointer allocate(size_type n) {
    return (n != 0 ? alloc.allocate(n) : pointer());
  }

  void deallocate(pointer p, size_type n) {
    if (p) {
      alloc.deallocate(p, n);
    }
  }

  ///
  /// @brief Construct an object with a given value at a given index.
  ///
  /// @param p
  /// @param val
  /// @param index
  /// @return pointer Returns a pointer to the constructed element
  ///
  pointer construct(pointer p, const value_type &val, size_type index = 0) {
    alloc.construct(p + index, val);
    return p + index;
  }

  template <class Ptr>
  typename enable_if<is_pointer<Ptr>::value, pointer>::type
  construct(Ptr p, Ptr first, Ptr last) {
    typedef typename ft::is_integral<Ptr>::type Integral;
    return resolveConstruct(p, first, last, Integral());
  }
  template <class Iter>
  typename enable_if<!is_pointer<Iter>::value, pointer>::type
  construct(Iter p, Iter first, Iter last) {
    typedef typename ft::is_integral<Iter>::type Integral;
    return resolveConstruct(&(*p), first, last, Integral());
  }
  template <class Ptr, class Iter>
  typename enable_if<is_pointer<Ptr>::value, pointer>::type
  construct(Ptr p, Iter first, Iter last) {
    typedef typename ft::is_integral<Iter>::type Integral;
    return resolveConstruct(p, first, last, Integral());
  }
  template <class Ptr, class Iter>
  typename enable_if<!is_pointer<Iter>::value, pointer>::type
  construct(Iter p, Ptr first, Ptr last) {
    typedef typename ft::is_integral<Iter>::type Integral;
    return resolveConstruct(&(*p), Iter(first), Iter(last), Integral());
  }
  template <class Ptr, class Iter>
  pointer construct(Ptr p, Ptr first, Iter last) {
    typedef typename ft::is_integral<Iter>::type Integral;
    return resolveConstruct(p, Iter(first), last, Integral());
  }
  template <class Ptr, class Iter>
  pointer construct(Ptr p, Iter first, Ptr last) {
    typedef typename ft::is_integral<Iter>::type Integral;
    return resolveConstruct(p, first, Iter(last), Integral());
  }

  ///
  /// @brief Destroys the object at the pointer index, deos not free memory.
  ///
  /// @param ptr
  /// @param i
  ///
  void destroy(pointer ptr, size_type i = 0) { alloc.destroy(ptr + i); }

  template<typename Iter>
  void destroy(Iter first, Iter last) {
    while(first != last) {
        alloc.destroy(&(*first));
        first++;
    }
  }

  ///
  /// @brief Destroys all objects within the vector, does not free memory.
  ///
  ///
  void destroyAll() {
    for (size_type i = 0; i < size(); i++) {
      destroy(start, i);
    }
  }

  ///
  /// @brief Returns the current size of the vector.
  ///
  /// @return size_type
  ///
  size_type size() const { return size_type(finish - start); }

  ///
  /// @brief Returns the maximum size available to allocate.
  ///
  /// @return size_type
  ///
  size_type max_size() const { return alloc.max_size(); }

  ///
  /// @brief Returns the current capacity of the vector.
  ///
  /// @return size_type
  ///
  size_type capacity() const { return size_type(endOfStorage - start); }

protected:
  void createStorage(size_type n) {
    start = this->allocate(n);
    finish = start;
    endOfStorage = start + n;
  }

  template <typename Integer>
  pointer resolveConstruct(pointer ptr, Integer val, Integer index, true_type) {
    alloc.construct(ptr + index, val);
    return ptr + index;
  }
  template <class Iter>
  pointer resolveConstruct(pointer ptr, Iter first, Iter last, false_type) {
    pointer result = ptr;
    while (first != last) {
      alloc.construct(result++, *first++);
    }
    return result;
  }
  template <>
  pointer resolveConstruct(pointer ptr, pointer first, pointer last, false_type) {
    pointer result = ptr;
    while (first != last) {
      alloc.construct(result++, *first++);
    }
    return result;
  }

  allocator_type alloc;
  pointer start;
  pointer finish;
  pointer endOfStorage;
};

template <class T, class Allocator = std::allocator<T> >
class vector : protected vectorBase<T, Allocator> {

public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type size_type;
  typedef typename allocator_type::difference_type difference_type;
  typedef ft::random_access_iterator<value_type> iterator;
  typedef ft::random_access_iterator<const value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef vectorBase<T, Allocator> Base;

  using Base::allocate;
  using Base::capacity;
  using Base::deallocate;
  using Base::max_size;
  using Base::size;
  using Base::get_allocator;

  /// ---------- Ctors & operators

  ///
  /// @brief Construct a new vector object -> Default
  ///
  /// @param alloc
  ///
  explicit vector(const allocator_type &alloc = allocator_type())
      : Base(alloc) {}

  ///
  /// @brief Construct a new vector object -> Fill constructor
  ///
  /// @param count Size of the vector
  /// @param value Value to fill the vector with
  /// @param alloc
  ///
  explicit vector(size_type count, const T &value = T(),
                  const allocator_type &alloc = Allocator())
      : Base(count, alloc) {
        Base::finish = ft::fill_n(Base::start, count, value);
  }

  ///
  /// @brief Construct a new vector object copied from another
  /// Copy constructor. Constructs the container with the copy of the contents
  /// of other.
  ///
  /// @param other
  ///
  vector(const vector &other) : Base(other.size(), other.get_allocator()) {
    Base::finish = construct(Base::start, other.begin(), other.end());
  }

  ///
  /// @brief Construct a new vector object through a range constructor
  ///
  /// @tparam InputIt
  /// @param first
  /// @param last
  /// @param alloc
  ///
  template <class InputIt>
  vector(InputIt first, InputIt last, const allocator_type &alloc = Allocator())
      : Base(alloc) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    // Gets caught by integer fill constructor if it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    initializeDispatch(first, last, Integral());
  }

  ///
  /// @brief Destroy the vector object
  /// First destroys all objects contained, then frees the memory (in Base).
  ///
  ~vector() {}

  ///
  /// @brief Copy Assignment operator
  ///
  /// @param other
  /// @return vector&
  ///
  vector &operator=(const vector &other) {
    if (&other != this) {
        destroyAll();
        deallocate(Base::start, capacity());
        Base::start = allocate(other.size());
        Base::finish = construct(Base::start, other.begin(), other.end());
        Base::endOfStorage = Base::finish;
    }
    return *this;
  }

  /// ---------- Iterators
  iterator begin() { return iterator(Base::start); }
  const_iterator begin() const { return const_iterator(Base::start); }
  iterator end() { return iterator(Base::finish); }
  const_iterator end() const { return const_iterator(Base::finish); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  /// ---------- Member Functions

  ///
  /// @brief Replaces the contents with b copies of value value.
  ///
  /// @param n
  /// @param val
  ///
  void assign(size_type n, value_type const &val) { fillAssign(n, val); }
  template <typename InputIt> void assign(InputIt first, InputIt last) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    assignDispatch(first, last, Integral());
  }

  ///
  /// @brief Resize the vector
  ///
  /// @param newSize
  /// @param val
  ///
  void resize(size_type newSize, const value_type &val = value_type()) {
    if (newSize > size()) {
        fillInsert(end(), newSize - size(), val);

    } else if (newSize < size()) {
      eraseUntilEnd(Base::start + newSize);
    }
  }

  ///
  /// @brief Check if the vector is empty.
  ///
  /// @return true
  /// @return false
  ///
  bool empty() const { return begin() == end(); }

  ///
  /// @brief Increase the capacity to n.
  ///
  /// @param n
  ///
  void reserve(size_type n) {
    if (n > this->max_size())
      throw std::length_error("vector::reserve");
    if (this->capacity() < n) {
      const size_type oldSize = size();
      pointer tmp = allocateAndCopy(n, iterator(Base::start), iterator(Base::finish));
      destroy(Base::start, Base::finish);
      deallocate(Base::start, capacity());
      Base::start = (tmp);
      Base::finish = (tmp + oldSize);
      Base::endOfStorage = (Base::start + n);
    }
  }

  ///
  /// @brief Return the element at index n.
  ///
  /// @param n
  /// @return reference
  ///
  reference operator[](size_type n) { return *(Base::start + n); }
  const_reference operator[](size_type n) const { return *(Base::start + n); }

  ///
  /// @brief Return the element at index n.
  ///
  /// @param n
  /// @return reference
  ///
  reference at(size_type n) {
    rangeCheck(n);
    return (*this)[n];
  }
  const_reference at(size_type n) const {
    rangeCheck(n);
    return (*this)[n];
  }

  ///
  /// @brief Return the first element.
  ///
  /// @return reference
  ///
  reference front() {
    requireNonEmpty();
    return *begin();
  }
  const_reference front() const {
    requireNonEmpty();
    return *begin();
  }

  ///
  /// @brief Return the last element.
  ///
  /// @return reference
  ///
  reference back() {
    requireNonEmpty();
    return *(end() - 1);
  }
  const_reference back() const {
    requireNonEmpty();
    return *(end() - 1);
  }

  ///
  /// @brief Return the data PTR.
  ///
  /// @return T*
  ///
  T *data() { return dataPtr(Base::start); }
  const T *data() const { return dataPtr(Base::start); }

  ///
  /// @brief Add the value at the end of the vector.
  ///
  /// @param val
  ///
  void push_back(const value_type &val) {
    if (Base::finish != Base::endOfStorage) {
      construct(Base::finish, val);
      ++Base::finish;
    } else
      reallocInsert(end(), val);
  }

  ///
  /// @brief Remove the last element.
  ///
  ///
  void pop_back() {
    requireNonEmpty();
    eraseUntilEnd(--Base::finish);
  }

  iterator insert(iterator position, const value_type &val) {
    fillInsert(position, 1, val);
    return  position--;
  }

  ///
  /// @brief
  ///
  /// @param position
  /// @param n
  /// @param val
  ///
  void insert(iterator position, size_type n, const value_type &val) {
    fillInsert(position, n, val);
  }

  template <typename InputIt>
  void insert(iterator position, InputIt first, InputIt last) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    insertDispatch(position, first, last, Integral());
  }

  iterator erase(iterator position) {
    if (position + 1 != end())
      std::copy(position + 1, end(), position);
    --Base::finish;
    destroy(Base::finish);
    return position;
  }
  iterator erase(iterator first, iterator last) {
    if (first != last) {
      if (last != end())
        std::copy(last, end(), first);
      eraseUntilEnd(&(*(first + (end() - last))));
    }
    return first;
  }

  void swap(vector &other) { swapData(other); }

  void clear() { erase(begin(), end()); }

protected:
  using Base::construct;
  using Base::copyData;
  using Base::destroy;
  using Base::destroyAll;
  using Base::swapData;

  void rangeCheck(size_type n) const {
    if (n >= this->size())
      std::__throw_out_of_range_fmt(__N("vector::_M_range_check: __n "
                                        "(which is %zu) >= this->size() "
                                        "(which is %zu)"),
                                    n, this->size());
  }

  void requireNonEmpty() { if (empty()) throw ft::ContainerIsEmptyError(); }

  template <typename ForwardIterator>
  pointer allocateAndCopy(size_type n, ForwardIterator first,
                          ForwardIterator last) {
    pointer result = allocate(n);
    try {
      construct(result, first, last);
      return result;
    } catch (...) {
      deallocate(result, n);
      __throw_exception_again;
    }
  }

  void reallocInsert(iterator position, const T &x) {
    size_type len = checkLen(1, "vector::reallocInsert");
    pointer oldStart = Base::start;
    pointer oldFinish = Base::finish;
    const size_type elemsBefore = position - begin();
    pointer newStart = allocate(len);
    pointer newFinish(newStart);
    construct(newStart + elemsBefore, x);
    newFinish = construct(newStart, oldStart, position);
    /// Jump over inserted value
    ++newFinish;
    newFinish = construct(newFinish, position, oldFinish);
    destroy(oldStart, oldFinish);
    deallocate(oldStart, Base::endOfStorage - oldStart);
    Base::start = (newStart);
    Base::finish = (newFinish);
    Base::endOfStorage = (newStart + len);
  }

  ///
  /// @brief Fill constructor specialization to catch integral calls.
  ///
  /// @tparam Integer
  /// @param n
  /// @param value
  ///
  template <typename Integer>
  void initializeDispatch(Integer n, Integer value, ft::true_type) {
    Base::start = (allocate(static_cast<size_type>(n)));
    Base::finish = (ft::fill_n(Base::start, static_cast<size_type>(n), value));
  }

  ///
  /// @brief InputIterator specialization to use the Range Constructor.
  ///
  /// @tparam InputIterator
  /// @param first
  /// @param last
  ///
  template <typename InputIterator>
  void initializeDispatch(InputIterator first, InputIterator last,
                          ft::false_type) {
    rangeInitialize(first, last);
  }

  ///
  /// @brief Initialize the vector with a given range.
  ///
  /// @tparam InputIterator
  /// @param first
  /// @param last
  ///
  template <typename InputIterator>
  void rangeInitialize(InputIterator first, InputIterator last) {
    for (; first != last; ++first)
      push_back(*first);
  }

  ///
  /// @brief Fill an n amount with the value given.
  ///
  /// @param n
  /// @param value
  ///
  void fillInitialize(size_type n, const value_type &value) {
    Base::finish = (ft::fill_n(Base::start, n, value));
  }

  template<typename InputIterator>
  void assignDispatch(InputIterator first, InputIterator last, ft::false_type) {
    assignHelper(first, last, ft::__iterator_category(first));
  }

  template <typename InputIterator>
  void assignHelper(InputIterator first, InputIterator last,
                    std::input_iterator_tag) {
                        assignHelper(first, last, ft::input_iterator_tag());
                    }

  template <typename ForwardIterator>
  void assignHelper(ForwardIterator first, ForwardIterator last,
                    std::forward_iterator_tag) {
                        assignHelper(first, last, ft::forward_iterator_tag());
                    }

  template <typename ForwardIterator>
  void assignHelper(ForwardIterator first, ForwardIterator last,
                    std::random_access_iterator_tag) {
                        assignHelper(first, last, ft::forward_iterator_tag());
                    }

  template <typename InputIterator>
  void assignHelper(InputIterator first, InputIterator last,
                    ft::input_iterator_tag) {
    pointer cur(Base::start);
    for (; first != last && cur != Base::finish; ++cur, ++first)
      *cur = *first;
    if (first == last)
      eraseUntilEnd(cur);
    else
      rangeInsert(end(), first, last, ft::__iterator_category(first));
  }

  template <typename ForwardIterator>
  void assignHelper(ForwardIterator first, ForwardIterator last,
                    ft::forward_iterator_tag) {
    const size_type len = ft::distance(first, last);
    if (len > capacity()) {
      destroyAll();
      deallocate(Base::start, capacity());
      Base::start = allocateAndCopy(len, first, last);
      Base::finish = Base::start + len;
      Base::endOfStorage = Base::finish;
    } else if (size() >= len)
      eraseUntilEnd(std::copy(first, last, Base::start));
    else {
      ForwardIterator mid = first;
      ft::advance(mid, size());
      std::copy(first, mid, Base::start);
      Base::finish = (construct(Base::finish, mid, last) + 1);
    }
  }

  ///
  /// @brief Catches the Integer resolution, resolves like normal assign.
  ///
  /// @tparam Integer
  /// @param n
  /// @param val
  ///
  template <typename Integer>
  void assignDispatch(Integer n, Integer val, ft::true_type) {
    fillAssign(n, val);
  }

  template <typename Integer>
  void insertDispatch(iterator pos, Integer n, Integer val, ft::true_type) {
    fillInsert(pos, n, val);
  }

  template <typename InputIterator>
  void insertDispatch(iterator pos, InputIterator first, InputIterator last,
                      ft::false_type) {
    rangeInsert(pos, first, last, ft::__iterator_category(first));
  }

  size_type checkLen(size_type n, const char *s) const {
    if (max_size() - size() < n)
      throw std::length_error(s);
    const size_type len = size() + std::max(size(), n);
    return (len < size() || len > max_size()) ? max_size() : len;
  }

  ///
  /// @brief Erases all objects from the pointer until the end.
  ///
  /// @param pos
  ///
  void eraseUntilEnd(pointer pos) {
    if (size_type n = Base::finish - pos) {
      destroy(pos, Base::finish);
      Base::finish = (pos);
    }
  }

  template <typename U> U *dataPtr(U *ptr) const { return ptr; }
  template <typename U> U *dataPtr(U *ptr) { return ptr; }
  template <typename Ptr> value_type *dataPtr(Ptr ptr) {
    return empty() ? (value_type *)0 : ptr.operator->();
  }
  template <typename Ptr> const value_type *dataPtr(Ptr ptr) const {
    return empty() ? (const value_type *)0 : ptr.operator->();
  }

  ///
  /// @brief Replace the vector with n values.
  ///
  /// @param n
  /// @param value
  ///
  void fillAssign(size_type n, const value_type &value) {
    if (n > capacity()) {
      destroyAll();
      deallocate(Base::start, capacity());
      Base::start = allocate(n);
      Base::finish = ft::fill_n(Base::start, n, value);
      Base::endOfStorage = Base::finish;
    } else if (n > size()) {
      ft::fill(begin(), end(), value);
      size_type add = n - size();
      Base::finish = (ft::fill_n(Base::finish, add, value));
    } else
      eraseUntilEnd(ft::fill_n(Base::start, n, value));
  }

  void fillInsert(iterator position, size_type n, const value_type &x) {
    if (n != 0) {
        // Enough storage.
      if (size_type(Base::endOfStorage - Base::finish) >= n) {
        value_type x_copy = x;
        const size_type elems_after = end() - position;
        pointer old_finish(Base::finish);
        // More elems after position than being inserted.
        if (elems_after > n) {
            // Copy the last n elems forward.
          Base::finish =
              construct(Base::finish, iterator(Base::finish - n), iterator(Base::finish));
          std::copy_backward(&(*position), old_finish - n, old_finish);
          // Fill in the inserted elems.
          ft::fill(position, position + n, x_copy);
        } else {
            // Fill the difference with the elems.
          Base::finish = ft::fill_n(Base::finish, n - elems_after, x_copy);
          // Copy position to old_finish to the end.
          Base::finish = construct(Base::finish, position, old_finish);
          // Fill in the inserted elems.
          ft::fill(&(*position), old_finish, x_copy);
        }
      } else {
        // Not enough storage, needs to be expanded.
        const size_type len = checkLen(n, "vector::fillInsert");
        const size_type elems_before = position - begin();
        pointer new_start(allocate(len));
        pointer new_finish(new_start);
        // Fill in the space after the elems with the inserted ones.
        ft::fill_n(new_start + elems_before, n, x);
        new_finish = pointer();
        // Fill in the elems_before.
        new_finish = construct(new_start, Base::start, position);
        // Fill in the rest of the elems after the inserted ones.
        new_finish = construct(new_finish, position, Base::finish);
        if (end() - position == 0) {
            new_finish = new_start + elems_before + n;
        }
        // Destroy and deallocate the old elems.
        destroy(Base::start, Base::finish);
        deallocate(Base::start, capacity());
        // Update the pointers to the new ones.
        Base::start = new_start;
        Base::finish = new_finish;
        Base::endOfStorage = new_start + len;
      }
    }
  }

  template <typename InputIterator>
  void rangeInsert(iterator pos, InputIterator first, InputIterator last,
                   ft::input_iterator_tag) {
    {
      for (; first != last; ++first) {
        pos = insert(pos, *first);
        ++pos;
      }
    }
  }

  template <typename ForwardIterator>
  void rangeInsert(iterator position, ForwardIterator first,
                   ForwardIterator last, ft::forward_iterator_tag) {
    if (first != last) {
      const size_type n = ft::distance(first, last);
      if (size_type(Base::endOfStorage - Base::finish) >= n) {
        const size_type elems_after = end() - position;
        pointer old_finish(Base::finish);
        if (elems_after > n) {
          Base::finish = construct(Base::finish, iterator(Base::finish - n),
                                   iterator(Base::finish));
          std::copy_backward(&(*position), old_finish - n, old_finish);
          std::copy(first, last, position);
        } else {
          ForwardIterator mid = first;
          ft::advance(mid, elems_after);
          Base::finish = construct(Base::finish, iterator(mid), iterator(last));
          Base::finish = construct(Base::finish, position, old_finish);
          std::copy(first, mid, position);
        }
      } else {
        const size_type len = checkLen(n, "vector::_M_range_insert");
        pointer new_start(allocate(len));
        pointer new_finish(new_start);
        new_finish = construct(new_start, Base::start, position);
        new_finish = construct(new_finish, iterator(first), iterator(last));
        new_finish = construct(new_finish, position, Base::finish);
        destroy(Base::start, Base::finish);
        deallocate(Base::start, Base::endOfStorage - Base::start);
        Base::start = new_start;
        Base::finish = new_finish;
        Base::endOfStorage = new_start + len;
      }
    }
  }
};

template <class T, class Alloc>
  bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {

  }

template <class T, class Alloc>
  bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

template <class T, class Alloc>
  bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

template <class T, class Alloc>
  bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

template <class T, class Alloc>
  bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

template <class T, class Alloc>
  bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

} // namespace ft

#endif