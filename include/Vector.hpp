/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/06/28 15:59:34 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Iterators.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
#include <memory>
#include <stdexcept>

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

  pointer getStartPtr() const { return start; }
  pointer getFinishPtr() const  { return finish; }
  pointer getEndOfStoragePtr() const { return endOfStorage; }
  void setStartPtr(pointer newStart) { start = newStart; }
  void setFinishPtr(pointer newFinish) { finish = newFinish; }
  void setEndOfStoragePtr(pointer newEndOfStorage) {
    endOfStorage = newEndOfStorage;
  }

  void copyData(const vectorBase &rhs) {
    setStartPtr(rhs.start);
    setFinishPtr(rhs.finish);
    setEndOfStoragePtr(rhs.endOfStorage);
    alloc = rhs.alloc;
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

  template <class Iter> pointer construct(pointer p, Iter first, Iter last) {
    typedef typename ft::is_integral<Iter>::type Integral;
    return resolveConstruct(p, first, last, Integral());
  }
  ///
  /// @brief Destroys the object at the pointer index, deos not free memory.
  ///
  /// @param ptr
  /// @param i
  ///
  void destroy(pointer ptr, size_type i = 0) { alloc.destroy(ptr + i); }

  ///
  /// @brief Destroys all objects within the vector, does not free memory.
  ///
  ///
  void destroyAll() {
    for (size_type i = 0; i < size(); i++) {
      destroy(getStartPtr(), i);
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
    pointer result;
    while (first != last) {
      result = ptr;
      alloc.construct(ptr++, *first++);
    }
    return result;
  }

private:
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
  typedef ft::const_random_access_iterator<value_type> const_iterator;
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
    setFinishPtr(ft::fill_n(getStartPtr(), count, value));
  }

  ///
  /// @brief Construct a new vector object copied from another
  /// Copy constructor. Constructs the container with the copy of the contents
  /// of other.
  ///
  /// @param other
  ///
  vector(const vector &other) : Base(other.capacity(), other.get_allocator()) {
    pointer ptr;
    for (size_type i = 0; i < other.size(); i++) {
      ptr = construct(getStartPtr(), other[i], i);
    }
    setFinishPtr(++ptr);
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
  /// if !=
  ///   if the other's size > current capacity
  ///       -> Allocate and copy a new pointer
  ///       -> destroy + deallocate current vector
  ///       -> getStartPtr() is the new pointer
  ///   if current size >= other's size
  ///       -> destroy all current stored objs
  ///   else
  ///       -> Copy construct other into current
  ///
  /// @param other
  /// @return vector&
  ///
  vector &operator=(const vector &other) {
    if (&other != this) {
      const size_type otherSize = other.size();
      if (otherSize > capacity()) {
        destroyAll();
        deallocate(getStartPtr(), capacity());
        pointer temp = allocate(otherSize);
        setStartPtr(temp);
        setFinishPtr(construct(temp, other.begin(), other.end()) + 1);
      } else if (size() >= otherSize) {
        destroyAll();
      } else {
        construct(getStartPtr(), other.begin(), other.end());
      }
    }
    return *this;
  }

  /// ---------- Iterators
  iterator begin() { return iterator(getStartPtr()); }
  const_iterator begin() const { return const_iterator(getStartPtr()); }
  iterator end() { return iterator(getFinishPtr()); }
  const_iterator end() const { return const_iterator(getFinishPtr()); }
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
      if (newSize > capacity()) {
        (newSize / 2 < size()) ? reserve(size() * 2) : reserve(newSize);
      }
    } else if (newSize < size())
      eraseAtEnd(getStartPtr() + newSize);
    pointer newFinish;
    for (size_type i = size; i < newSize; i++) {
      newFinish = construct(getStartPtr(), val, i);
    }
    setFinishPtr(newFinish);
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
    if (n <= capacity()) {
      return;
    } else if (n > max_size()) {
      throw std::length_error("vector::reserve");
    }
    pointer temp = allocate(n);
    pointer newFinish = construct(temp, begin(), end()) + 1;
    destroyAll();
    deallocate(getStartPtr(), capacity());
    setStartPtr(temp);
    setFinishPtr(newFinish);
    setEndOfStoragePtr(getStartPtr() + n);
  }

  ///
  /// @brief Return the element at index n.
  ///
  /// @param n
  /// @return reference
  ///
  reference operator[](size_type n) { return *(getStartPtr() + n); }
  const_reference operator[](size_type n) const { return *(getStartPtr() + n); }

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
  T *data() { return dataPtr(getStartPtr()); }
  const T *data() const { return dataPtr(getStartPtr()); }

  ///
  /// @brief Add the value at the end of the vector.
  ///
  /// @param val
  ///
  void push_back(const value_type &val) {
    if (empty()) {
      reserve(1);
    } else if (size() == capacity()) {
      (size() * 2 < max_size()) ? reserve(size() * 2) : reserve(max_size());
    }
    pointer newFinish = construct(getFinishPtr(), val) + 1;
    setFinishPtr(newFinish);
  }

  ///
  /// @brief Remove the last element.
  ///
  ///
  void pop_back() {
    requireNonEmpty();
    eraseUntilEnd(--getFinishPtr());
  }

  ///
  /// @brief
  ///
  /// @param position
  /// @param val
  ///
  void insert(iterator position, const value_type &val) {
    fillInsert(position, 1, val);
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
    setFinishPtr(--getFinishPtr());
    --this->_M_impl._M_finish;
    destroyObj(getFinishPtr());
    return position;
  }
  iterator erase(iterator first, iterator last) {
    if (first != last) {
      if (last != end())
        std::copy(last, end(), first);
      eraseUntilEnd(first.base() + (end() - last));
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
  using Base::getEndOfStoragePtr;
  using Base::getFinishPtr;
  using Base::getStartPtr;
  using Base::setEndOfStoragePtr;
  using Base::setFinishPtr;
  using Base::setStartPtr;
  using Base::swapData;

  void rangeCheck(size_type n) const {
    if (n >= this->size())
        throw std::out_of_range("vector");
  }

  void requireNonEmpty() { if (!empty()) throw ft::ContainerIsEmptyError(); }

  ///
  /// @brief Fill constructor specialization to catch integral calls.
  ///
  /// @tparam Integer
  /// @param n
  /// @param value
  ///
  template <typename Integer>
  void initializeDispatch(Integer n, Integer value, ft::true_type) {
    setStartPtr(allocate(static_cast<size_type>(n)));
    setFinishPtr(ft::fill_n(getStartPtr(), static_cast<size_type>(n), value));
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
    setFinishPtr(ft::fill_n(getStartPtr(), n, value));
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
    if (size_type n = getFinishPtr() - pos) {
      for (pointer ptr = pos; ptr != getFinishPtr(); ptr++) {
        destroy(ptr);
      }
      setFinishPtr(pos);
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
      vector tmp(n, value, get_allocator());
      swapData(*this);
    } else if (n > size()) {
      ft::fill(begin(), end(), value);
      size_type add = n - size();
      setFinishPtr(ft::fill_n(getFinishPtr(), add, value));
    } else
      eraseUntilEnd(ft::fill_n(getStartPtr(), n, value));
  }

  void fillInsert(iterator position, size_type n, const value_type &x) {
    if (n != 0) {
      if (size_type(getEndOfStoragePtr() - getFinishPtr()) >= n) {
        value_type xCopy = x;
        const size_type elemsAfter = end() - position;
        pointer oldFinish(getFinishPtr());
        if (elemsAfter > n) {
          construct(getFinishPtr(), iterator(getFinishPtr() - n),
                    iterator(getFinishPtr()));
          getFinishPtr() += n;
          ft::fill(position, position + n, xCopy);
        } else {
          getFinishPtr() = ft::fill_n(getFinishPtr(), n - elemsAfter, xCopy);
          construct(position, iterator(oldFinish), iterator(getFinishPtr()));
          getFinishPtr() += elemsAfter;
          ft::fill(position, oldFinish, xCopy);
        }
      } else {
        const size_type len = checkLen(n, "vector::_M_fill_insert");
        const size_type elemsBefore = position - begin();
        pointer newStart(allocate(len));
        pointer newFinish(newStart);
        ft::fill_n(newStart + elemsBefore, n, x);
        newFinish = pointer();
        newFinish = construct(getStartPtr(), position, newStart) + 1;
        newFinish += n;
        newFinish = construct(position, getFinishPtr(), newFinish) + 1;
        destroyAll();
        deallocate(getStartPtr(), getEndOfStoragePtr() - getStartPtr());
        setStartPtr(newStart);
        setFinishPtr(newFinish);
        setEndOfStoragePtr(newStart + len);
      }
    }
  }

  template <typename InputIterator>
  void rangeInsert(iterator pos, InputIterator first, InputIterator last,
                   std::input_iterator_tag) {
    {
      for (; first != last; ++first) {
        pos = insert(pos, *first);
        ++pos;
      }
    }
  }

  template <typename ForwardIterator>
  void rangeInsert(iterator position, ForwardIterator first,
                   ForwardIterator last, std::forward_iterator_tag) {
    if (first != last) {
      const size_type n = ft::distance(first, last);
      if (size_type(getEndOfStoragePtr() - getFinishPtr()) >= n) {
        const size_type elemsAfter = end() - position;
        pointer oldFinish(getFinishPtr());
        if (elemsAfter > n) {
          construct(getFinishPtr() - n, getFinishPtr(), getFinishPtr());
          getFinishPtr() += n;
          std::copy_backward(position, oldFinish - n, oldFinish);
          std::copy(first, last, position);
        } else {
          ForwardIterator mid = first;
          ft::advance(mid, elemsAfter);
          construct(mid, last, getFinishPtr());
          getFinishPtr() += n - elemsAfter;
          construct(position.base(), oldFinish, getFinishPtr());
          getFinishPtr() += elemsAfter;
          std::copy(first, mid, position);
        }
      } else {
        const size_type len = checkLen(n, "vector::rangeInsert");
        pointer newStart(allocate(len));
        pointer newFinish(newStart);
        newFinish =
            construct(getStartPtr(), position.base(), newStart);
        newFinish = construct(first, last, newFinish);
        newFinish = construct(position.base(), getFinishPtr(), newFinish);
        destroyAll();
        deallocate(getStartPtr(), getEndOfStoragePtr() - getStartPtr());
        getStartPtr() = newStart;
        getFinishPtr() = newFinish;
        getEndOfStoragePtr() = newStart + len;
      }
    }
  }
};

} // namespace ft

#endif