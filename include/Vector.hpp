/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/04/08 18:18:25 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "AllocatorTraits.hpp"
#include "Iterators.hpp"
#include <cassert>
#include <cstddef>
#include <memory>
#define assertm(exp, msg) assert(((void)msg, exp))

namespace ft {

template <class T, class Allocator = std::allocator<T>> struct vectorBase {
  // Implicit vector data
  typedef typename ft::alloc_traits<Allocator>::template rebind<T>::other
      TAllocType;
  typedef typename ft::alloc_traits<Allocator>::pointer pointer;
  struct vectorImplData {
    pointer start;
    pointer finish;
    pointer endOfStorage;
    vectorImplData() : start(), finish(), endOfStorage() {}
    void copyData(const vectorImplData &rhs) {
      start = rhs.start;
      finish = rhs.finish;
      endOfStorage = rhs.endOfStorage;
    }
    void swapData(vectorImplData &rhs) {
      vectorImplData tmp;
      tmp.copyData(*this);
      copyData(rhs);
      rhs.copyData(tmp);
    }
  };
  struct vectorImpl : public TAllocType, public vectorImplData {
    vectorImpl() : TAllocType() {}
    vectorImpl(TAllocType const &src) : TAllocType(src) {}
  };

public:
  typedef Allocator allocator_type;
  vectorImpl implV;
  TAllocType &getTAllocator() { return this->implV; }
  TAllocType const &getTAllocator() const { return this->implV; }
  allocator_type get_allocator() const {
    return allocator_type(getTAllocator());
  }
  vectorBase() {}
  vectorBase(allocator_type const &al) : implV(al) {}
  vectorBase(size_t n, allocator_type const &al) : implV(al) {
    createStorage(n);
  }
  ~vectorBase() { deallocate(implV.start, implV.endOfStorage - implV.start); }
  pointer allocate(size_t n) {
    return (n != 0 ? TAllocType::allocate(implV, n) : pointer());
  }
  void deallocate(pointer p, size_t n) {
    if (p) {
      TAllocType::deallocate(implV, p, n);
    }
  }

protected:
  void createStorage(size_t n) {
    this->implV.start = this->allocate(n);
    this->implV.finish = this->implV.start;
    this->implV.endOfStorage = this->implV.start + n;
  }
};

template <class T, class Allocator = std::allocator<T>>
class vector : protected vectorBase<T, Allocator> {
  typedef vectorBase<T, Allocator> Base;
  typedef typename Base::TAllocType TAllocType;
  typedef ft::alloc_traits<TAllocType> AllocTraits;

public:
  typedef T value_type;
  typedef typename Base::pointer pointer;
  typedef typename AllocTraits::const_pointer const_pointer;
  typedef typename AllocTraits::reference reference;
  typedef typename AllocTraits::const_reference const_reference;
  typedef ft::random_access_iterator<vector> iterator;
  typedef ft::const_random_access_iterator<vector> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef Allocator allocator_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  vector() {}
  explicit vector(const allocator_type &alloc = allocator_type())
      : Base(alloc) {}
  explicit vector(size_type count, const T &value = T(),
                  const allocator_type &alloc = Allocator())
      : Base(checkInitLen(count, alloc)) {
    fillInitialize(count, value);
  }
  vector(const vector &other)
      : Base(other.size(), AllocTraits::select_on_copy(other.getTAllocator)) {
    this->implV.finish =
        std::uninitialized_copy(other.begin(), other.end(), this->implV.start);
  }
  template <class InputIt>
  vector(InputIt first, InputIt last, const allocator_type &alloc = Allocator())
      : Base(alloc) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    initializeDispatch(first, last, Integral());
  }
  ~vector() {
    for (pointer tmp = this->implV.start; tmp != this->implV.finish; tmp++) {
      AllocTraits::destroy(get_allocator(), tmp);
    }
  }
  vector &operator=(vector const &rhs);
  void assign(size_type n, value_type const &val) { fillAssign(n, val); }
  template <typename InputIt> void assign(InputIt first, InputIt last) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    assignDispatch(first, last, Integral());
  }
  // get_allocator
  using Base::get_allocator;

  // iterators
  iterator begin() { return iterator(this->implV.start); }
  const_iterator begin() const { return const_iterator(this->implV.start); }
  iterator end() { return iterator(this->implV.finish); }
  const_iterator end() const { return const_iterator(this->implV.finish); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  size_type size() const {
    return size_type(this->implV.finish - this->implV.start);
  }
  size_type max_size() const { return _S_max_size(getTAllocator()); }
  void resize(size_type newSize, value_type val = value_type()) {
    if (newSize > size())
      fillInsert(end(), newSize - size(), val);
    else if (newSize < size())
      eraseAtEnd(this->implV.start + newSize);
  }
  size_type capacity() const {
    return size_type(this->implV.endOfStorage - this->implV.start);
  }
  bool empty() const { return begin() == end(); }
  void reserve(size_type n);
  reference operator[](size_type n) { return *(this->implV.start + n); }
  const_reference operator[](size_type n) const {
    return *(this->implV.start + n);
  }
  reference at(size_type n) {
    rangeCheck(n);
    return (*this)[n];
  }
  const_reference at(size_type n) const {
    rangeCheck(n);
    return (*this)[n];
  }
  reference front() {
    requireNonEmpty();
    return *begin();
  }
  const_reference front() const {
    requireNonEmpty();
    return *begin();
  }
  reference back() {
    requireNonEmpty();
    return *(end() - 1);
  }
  const_reference back() const {
    requireNonEmpty();
    return *(end() - 1);
  }
  T *data() { return dataPtr(this->implV.start); }
  const T *data() const { return dataPtr(this->implV.start); }
  void push_back(const value_type &val) {
    if (this->implV.finish != this->implV.endOfStorage) {
      allocator_type::construct(this->implV.finish, val);
      ++this->implV.finish;
    } else
      reallocInsert(end(), val);
  }
  void pop_back() {
    requireNonEmpty();
    --this->implV.finish;
    AllocTraits::destroy(this->implV, this->implV.finish);
  }
  iterator insert(iterator position, const value_type &val);
  void insert(iterator position, size_type n, const value_type &val) {
    fillInsert(position, n, val);
  }
  template <typename InputIt>
  void insert(iterator position, InputIt first, InputIt last) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    insertDispatch(position, first, last, Integral());
  }
  iterator erase(iterator position) { return eraseV(position); }
  iterator erase(iterator first, iterator last) { return eraseV(first, last); }
  void swap(vector &other) {
    this->implV.swapData(other.implV);
    AllocTraits::_S_on_swap(getTAllocator(), other.getTAllocator());
  }
  void clear() { eraseAtEnd(this->implV.start); }

protected:
  using Base::allocate;
  using Base::deallocate;
  using Base::getTAllocator;
  using Base::implV;
  void rangeCheck(size_type n) const {
    if (n >= this->size()) {
      __throw_out_of_range_fmt(__N("vector::rangeCheck: n "
                                   "(which is %zu) >= this->size() "
                                   "(which is %zu)"),
                               n, this->size());
    }
  }
  void requireNonEmpty() { assertm(!empty(), "vector is empty"); }

private:
  size_type _size;
  size_type _nElements;
};

} // namespace ft

#endif