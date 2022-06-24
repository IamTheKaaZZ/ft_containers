/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/06/24 15:54:02 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Iterators.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
#include <memory>
#include <stdexcept>
#define assertm(exp, msg) assert(((void)msg, exp))

namespace ft {

// template <class T, class Allocator = std::allocator<T>> struct vectorBase {
//   // Implicit vector data
//   typedef typename ft::alloc_traits<Allocator>::template rebind<T>::other
//       TAllocType;
//   typedef typename ft::alloc_traits<Allocator>::pointer pointer;
//   struct vectorImplData {
//     pointer start;
//     pointer finish;
//     pointer endOfStorage;
//     vectorImplData() : start(), finish(), endOfStorage() {}
//     void copyData(const vectorImplData &rhs) {
//       start = rhs.start;
//       finish = rhs.finish;
//       endOfStorage = rhs.endOfStorage;
//     }
//     void swapData(vectorImplData &rhs) {
//       vectorImplData tmp;
//       tmp.copyData(*this);
//       copyData(rhs);
//       rhs.copyData(tmp);
//     }
//   };
//   struct vectorImpl : public TAllocType, public vectorImplData {
//     vectorImpl() : TAllocType() {}
//     vectorImpl(TAllocType const &src) : TAllocType(src) {}
//   };

// public:
//   typedef Allocator allocator_type;
//   vectorImpl implV;
//   TAllocType &getTAllocator() { return this->implV; }
//   TAllocType const &getTAllocator() const { return this->implV; }
//   allocator_type get_allocator() const {
//     return allocator_type(getTAllocator());
//   }
//   vectorBase() {}
//   vectorBase(allocator_type const &al) : implV(al) {}
//   vectorBase(size_t n, allocator_type const &al) : implV(al) {
//     createStorage(n);
//   }
//   ~vectorBase() { deallocate(implV.start, implV.endOfStorage - implV.start);
//   } pointer allocate(size_t n) {
//     return (n != 0 ? TAllocType::allocate(implV, n) : pointer());
//   }
//   void deallocate(pointer p, size_t n) {
//     if (p) {
//       TAllocType::deallocate(implV, p, n);
//     }
//   }

// protected:
//   void createStorage(size_t n) {
//     _start = this->allocate(n);
//     _start + size() = _start;
//     this->implV.endOfStorage = _start + n;
//   }
// };

template <class T, class Allocator = std::allocator<T>> class vector {

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
  typedef ft::const_random_access_iterator<const value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  ///
  /// @brief Assert if the allocator type is correct
  ///
  ///
  static_assert(
      (ft::is_same<typename allocator_type::value_type, value_type>::value),
      "Allocator::value_type must be same type as value_type");

  vector() {}

  ///
  /// @brief Construct a new vector object -> Default
  ///
  /// @param alloc
  ///
  explicit vector(const allocator_type &alloc = allocator_type())
      : _alloc(alloc), _size(0), _start(NULL), _capacity(0) {}

  ///
  /// @brief Construct a new vector object -> Fill constructor
  ///
  /// @param count Size of the vector
  /// @param value Value to fill the vector with
  /// @param alloc
  ///
  explicit vector(size_type count, const T &value = T(),
                  const allocator_type &alloc = Allocator())
      : _alloc(alloc), _size(count), _start(_alloc.allocate(count)),
        _capacity(count) {
    for (size_type i = 0; i < count; i++) {
      _alloc.construct(_start + i, value);
    }
  }

  ///
  /// @brief Construct a new vector object copied from another
  /// Copy constructor. Constructs the container with the copy of the contents
  /// of other.
  ///
  /// @param other
  ///
  vector(const vector &other)
      : _alloc(other._alloc), _size(other._size),
        _start(_alloc.allocate(_size)), _capacity(other._size) {
    for (size_type i = 0; i < _size; i++) {
      _alloc.construct(_start + i, other[i]);
    }
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
      : _alloc(alloc) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    // Gets caught by integer fill constructor if it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    initializeDispatch(first, last, Integral());
  }

  ///
  /// @brief Destroy the vector object
  /// First destroys all objects contained, then frees the memory.
  ///
  ~vector() {
    destroyAll();
    _alloc.deallocate(_start, _capacity);
  }

  ///
  /// @brief Copy Assignment operator
  /// if !=
  ///   if the other's size > current capacity
  ///       -> Allocate and copy a new pointer
  ///       -> destroy + deallocate current vector
  ///       -> _start is the new pointer
  ///   if current size >= other's size
  ///       -> destroy all current stored objs
  ///   else
  ///       -> Copy construct other into current
  /// 
  /// @param other 
  /// @return vector& 
  ///
  vector& operator=(const vector& other) {
    if (&other != this) {
      const size_type otherSize = other.size();
      if (otherSize > _capacity) {
        pointer temp = allocateAndCopy(otherSize, other.begin(), other.end());
        destroyAll();
        _alloc.deallocate(_start, _capacity);
        _start = temp;
        _size = otherSize;
        _capacity = otherSize;
      } else if (size() >= otherSize) {
        destroyAll();
      } else {
        copyConstruct(_start, other.begin(), other.end());
      }
      _size = otherSize;
    }
    return *this;
  }

  // Iterators
  iterator begin() { return iterator(_start); }
  const_iterator begin() const { return const_iterator(_start); }
  iterator end() { return iterator(_start + size()); }
  const_iterator end() const { return const_iterator(_start + size()); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  // Member Functions
  void assign(size_type n, value_type const &val) { fillAssign(n, val); }

  template <typename InputIt> void assign(InputIt first, InputIt last) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename ft::is_integral<InputIt>::type Integral;
    assignDispatch(first, last, Integral());
  }

  ///
  /// @brief Returns the current size of the vector.
  /// 
  /// @return size_type 
  ///
  size_type size() const {
    return size_type(_size);
  }

  ///
  /// @brief Returns the maximum size available to allocate.
  /// 
  /// @return size_type 
  ///
  size_type max_size() const { return _alloc.max_size(); }

  ///
  /// @brief Resize the vector
  /// 
  /// @param newSize 
  /// @param val 
  ///
  void resize(size_type newSize, const value_type& val = value_type()) {
    if (newSize > size()) {
        if (newSize > _capacity) {
            (newSize / 2 < _size) ? reserve(_size * 2) : reserve(newSize);
        }
    }
    else if (newSize < size())
      eraseAtEnd(_start + newSize);
    for (size_type i = _size; i < newSize; i++) {
        copyConstruct(_start, val, i);
    }
  }

  ///
  /// @brief Returns the current capacity of the vector.
  /// 
  /// @return size_type 
  ///
  size_type capacity() const {
    return size_type(_capacity);
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
    if (n <= _capacity) {
        return ;
    } else if (n > max_size()) {
        throw std::length_error("vector::reserve");
    }
    pointer temp = allocateAndCopy(n, begin(), end());
    destroyAll();
    _alloc.deallocate(_start, _capacity);
    _start = temp;
    _capacity = n;
  }

  ///
  /// @brief Return the element at index n.
  /// 
  /// @param n 
  /// @return reference 
  ///
  reference operator[](size_type n) { return *(_start + n); }
  const_reference operator[](size_type n) const {
    return *(_start + n);
  }

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
  T *data() { return dataPtr(_start); }
  const T *data() const { return dataPtr(_start); }

  ///
  /// @brief Add the value at the end of the vector.
  /// 
  /// @param val 
  ///
  void push_back(const value_type &val) {
    if (empty()) {
        reserve(1);
     } else if (_size == _capacity) {
      (_size * 2 < max_size()) ? reserve(_size * 2) : reserve(max_size());
     }
     copyConstruct(_start, val, _size++);
  }    

  ///
  /// @brief Remove the last element.
  /// 
  ///
  void pop_back() {
    requireNonEmpty();
    destroyObj(_start, --_size);
  }

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
    AllocTraits::on_swap(getTAllocator(), other.getTAllocator());
  }
  void clear() { eraseAtEnd(_start); }

protected:

  ///
  /// @brief Destroys the object at the pointer index, deos not free memory.
  /// 
  /// @param ptr 
  /// @param i 
  ///
  void destroyObj(pointer ptr, size_type i) {
    _alloc.destroy(ptr + i);
  }

  ///
  /// @brief Destroys all objects within the vector, does not free memory.
  /// 
  ///
  void destroyAll() {
    for (size_type i = 0; i < _size; i++) {
        _alloc.destroy(_start + i);
    }
  }

  void rangeCheck(size_type n) const {
    static_assert(n >= this->size(), "Invalid Range");
  }

  void requireNonEmpty() { static_assert(!empty(), "Vector is empty"); }

  ///
  /// @brief Copy Constructs a range in the dest pointer.
  /// 
  /// @param dest 
  /// @param start 
  /// @param end 
  ///
  void copyConstruct(pointer dest, iterator start, iterator end) {
    while (start != end) {
        _alloc.construct(dest++, *start++);
    }
  }

  ///
  /// @brief Copy Construct a specific index in the dest pointer
  /// 
  /// @param dest 
  /// @param value 
  /// @param i 
  ///
  void copyConstruct(pointer dest, const value_type& value, size_type i = 0) {
    _alloc.construct(dest + i, value);
  }

  ///
  /// @brief Allocates and Copy constructs a range
  /// 
  /// @tparam ForwardIterator 
  /// @param n 
  /// @param first 
  /// @param last 
  /// @return pointer Allocated and sopy constructed pointer
  ///
  template <typename ForwardIterator>
  pointer allocateAndCopy(size_type n, ForwardIterator first,
                          ForwardIterator last) {
    pointer result = _alloc.allocate(n);
    try {
      copyConstruct(result, first, last);
      return result;
    } catch (...) {
      _alloc.deallocate(result, n);
      throw;
    }
  }

  void copyData(const vector &rhs) {
      _start = rhs._start;
      _size = rhs._size;
      _capacity = rhs._capacity;
    }

  void swapData(vector &rhs) {
      vector tmp;
      tmp.copyData(*this);
      copyData(rhs);
      rhs.copyData(tmp);
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
    _start = _alloc.allocate(static_cast<size_type>(n));
    fillInitialize(static_cast<size_type>(n), value);
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
    for (size_type i = 0; i < n; i++) {
      _alloc.construct(_start + i, value);
    }
  }

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
  void eraseAtEnd(pointer pos) {
    if (size_type n = _start + size() - pos) {
      _alloc.destroy(pos);
      _start + size() = pos;
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
  
  void fillAssign(size_type n, const value_type & value) {
    if (n > capacity()) {
      vector tmp(n, value, _alloc);
      tmp.swapData(*this);
    } else if (n > size()) {
      std::fill(begin(), end(), value);
      size_type add = n - size();
      std::fill_n(_start + size(), add, value);
    } else
      eraseAtEnd(std::fill_n(_start, n, value));
  }

  template <typename InputIterator>
  void assignAux(InputIterator first, InputIterator last, std::input_iterator_tag) {
    pointer current(_start);
    for (; first != last && current != _start + size(); ++current, ++first)
      *current = *first;
    if (first == last)
      eraseAtEnd(current);
    else
      _M_range_insert(end(), __first, __last,
                      std::__iterator_category(__first));
  }

  template <typename ForwardIterator>
  void assignAux(ForwardIterator first, ForwardIterator last, std::forward_iterator_tag) {
    const size_type __len = std::distance(__first, __last);
    if (__len > capacity()) {
      pointer __tmp(allocateAndCopy(__len, __first, __last));
      _GLIBCXX_ASAN_ANNOTATE_REINIT;
      std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                    _M_get_Tp_allocator());
      _M_deallocate(this->_M_impl._M_start,
                    this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
      this->_M_impl._M_start = __tmp;
      this->_M_impl._M_finish = this->_M_impl._M_start + __len;
      this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    } else if (size() >= __len)
      _M_erase_at_end(std::copy(__first, __last, this->_M_impl._M_start));
    else {
      _ForwardIterator __mid = __first;
      std::advance(__mid, size());
      std::copy(__first, __mid, this->_M_impl._M_start);
      const size_type __attribute__((__unused__)) __n = __len - size();
      _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
      this->_M_impl._M_finish = std::__uninitialized_copy_a(
          __mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
    }
  }
  template <typename _Tp, typename _Alloc>
  void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp &__x) {
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                             _GLIBCXX_MOVE(*(this->_M_impl._M_finish - 1)));
    ++this->_M_impl._M_finish;
    _Tp __x_copy = __x;
    *__position = __x_copy;
    *__position = std::forward<_Arg>(__arg);
  }
  template <typename _Tp, typename _Alloc>
  void vector<_Tp, _Alloc>::_M_realloc_insert(iterator __position,
                                              const _Tp &__x) {
    const size_type __len =
        _M_check_len(size_type(1), "vector::_M_realloc_insert");
    pointer __old_start = this->_M_impl._M_start;
    pointer __old_finish = this->_M_impl._M_finish;
    const size_type __elems_before = __position - begin();
    pointer __new_start(this->_M_allocate(__len));
    pointer __new_finish(__new_start);
    __try {
      // The order of the three operations is dictated by the C++11
      // case, where the moves could alter a new element belonging
      // to the existing vector.  This is an issue only for callers
      // taking the element by lvalue ref (see last bullet of C++11
      // [res.on.arguments]).
      _Alloc_traits::construct(this->_M_impl, __new_start + __elems_before,
                               __x);
      __new_finish = pointer();
      __new_finish = std::__uninitialized_move_if_noexcept_a(
          __old_start, __position.base(), __new_start, _M_get_Tp_allocator());
      ++__new_finish;
      __new_finish = std::__uninitialized_move_if_noexcept_a(
          __position.base(), __old_finish, __new_finish, _M_get_Tp_allocator());
    }
    __catch(...) {
      if (!__new_finish)
        _Alloc_traits::destroy(this->_M_impl, __new_start + __elems_before);
      else
        std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
      _M_deallocate(__new_start, __len);
      __throw_exception_again;
    }
    _GLIBCXX_ASAN_ANNOTATE_REINIT;
    std::_Destroy(__old_start, __old_finish, _M_get_Tp_allocator());
    _M_deallocate(__old_start, this->_M_impl._M_end_of_storage - __old_start);
    this->_M_impl._M_start = __new_start;
    this->_M_impl._M_finish = __new_finish;
    this->_M_impl._M_end_of_storage = __new_start + __len;
  }
  template <typename _Tp, typename _Alloc>
  void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n,
                                           const value_type &__x) {
    if (__n != 0) {
      if (size_type(this->_M_impl._M_end_of_storage -
                    this->_M_impl._M_finish) >= __n) {
        value_type __x_copy = __x;
        const size_type __elems_after = end() - __position;
        pointer __old_finish(this->_M_impl._M_finish);
        if (__elems_after > __n) {
          std::__uninitialized_move_a(
              this->_M_impl._M_finish - __n, this->_M_impl._M_finish,
              this->_M_impl._M_finish, _M_get_Tp_allocator());
          this->_M_impl._M_finish += __n;
          std::fill(__position.base(), __position.base() + __n, __x_copy);
        } else {
          this->_M_impl._M_finish = std::__uninitialized_fill_n_a(
              this->_M_impl._M_finish, __n - __elems_after, __x_copy,
              _M_get_Tp_allocator());
          std::__uninitialized_move_a(__position.base(), __old_finish,
                                      this->_M_impl._M_finish,
                                      _M_get_Tp_allocator());
          this->_M_impl._M_finish += __elems_after;
          std::fill(__position.base(), __old_finish, __x_copy);
        }
      } else {
        const size_type __len = _M_check_len(__n, "vector::_M_fill_insert");
        const size_type __elems_before = __position - begin();
        pointer __new_start(this->_M_allocate(__len));
        pointer __new_finish(__new_start);
        __try {
          // See _M_realloc_insert above.
          std::__uninitialized_fill_n_a(__new_start + __elems_before, __n, __x,
                                        _M_get_Tp_allocator());
          __new_finish = pointer();
          __new_finish = std::__uninitialized_move_if_noexcept_a(
              this->_M_impl._M_start, __position.base(), __new_start,
              _M_get_Tp_allocator());
          __new_finish += __n;
          __new_finish = std::__uninitialized_move_if_noexcept_a(
              __position.base(), this->_M_impl._M_finish, __new_finish,
              _M_get_Tp_allocator());
        }
        __catch(...) {
          if (!__new_finish)
            std::_Destroy(__new_start + __elems_before,
                          __new_start + __elems_before + __n,
                          _M_get_Tp_allocator());
          else
            std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
          _M_deallocate(__new_start, __len);
          __throw_exception_again;
        }
        std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                      _M_get_Tp_allocator());
        _M_deallocate(this->_M_impl._M_start,
                      this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = __new_start;
        this->_M_impl._M_finish = __new_finish;
        this->_M_impl._M_end_of_storage = __new_start + __len;
      }
    }
  }
  template <typename _Tp, typename _Alloc>
  template <typename _InputIterator>
  void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos,
                                            _InputIterator __first,
                                            _InputIterator __last,
                                            std::input_iterator_tag) {
    if (__pos == end()) {
      for (; __first != __last; ++__first)
        insert(end(), *__first);
    } else if (__first != __last) {
      vector __tmp(__first, __last, _M_get_Tp_allocator());
      insert(__pos, _GLIBCXX_MAKE_MOVE_ITERATOR(__tmp.begin()),
             _GLIBCXX_MAKE_MOVE_ITERATOR(__tmp.end()));
    }
  }
  template <typename _Tp, typename _Alloc>
  template <typename _ForwardIterator>
  void vector<_Tp, _Alloc>::_M_range_insert(iterator __position,
                                            _ForwardIterator __first,
                                            _ForwardIterator __last,
                                            std::forward_iterator_tag) {
    if (__first != __last) {
      const size_type __n = std::distance(__first, __last);
      if (size_type(this->_M_impl._M_end_of_storage -
                    this->_M_impl._M_finish) >= __n) {
        const size_type __elems_after = end() - __position;
        pointer __old_finish(this->_M_impl._M_finish);
        if (__elems_after > __n) {
          _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
          std::__uninitialized_move_a(
              this->_M_impl._M_finish - __n, this->_M_impl._M_finish,
              this->_M_impl._M_finish, _M_get_Tp_allocator());
          this->_M_impl._M_finish += __n;
          _GLIBCXX_ASAN_ANNOTATE_GREW(__n);
          _GLIBCXX_MOVE_BACKWARD3(__position.base(), __old_finish - __n,
                                  __old_finish);
          std::copy(__first, __last, __position);
        } else {
          _ForwardIterator __mid = __first;
          std::advance(__mid, __elems_after);
          _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
          std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish,
                                      _M_get_Tp_allocator());
          this->_M_impl._M_finish += __n - __elems_after;
          _GLIBCXX_ASAN_ANNOTATE_GREW(__n - __elems_after);
          std::__uninitialized_move_a(__position.base(), __old_finish,
                                      this->_M_impl._M_finish,
                                      _M_get_Tp_allocator());
          this->_M_impl._M_finish += __elems_after;
          _GLIBCXX_ASAN_ANNOTATE_GREW(__elems_after);
          std::copy(__first, __mid, __position);
        }
      } else {
        const size_type __len = _M_check_len(__n, "vector::_M_range_insert");
        pointer __new_start(this->_M_allocate(__len));
        pointer __new_finish(__new_start);
        __try {
          __new_finish = std::__uninitialized_move_if_noexcept_a(
              this->_M_impl._M_start, __position.base(), __new_start,
              _M_get_Tp_allocator());
          __new_finish = std::__uninitialized_copy_a(
              __first, __last, __new_finish, _M_get_Tp_allocator());
          __new_finish = std::__uninitialized_move_if_noexcept_a(
              __position.base(), this->_M_impl._M_finish, __new_finish,
              _M_get_Tp_allocator());
        }
        __catch(...) {
          std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
          _M_deallocate(__new_start, __len);
          __throw_exception_again;
        }
        _GLIBCXX_ASAN_ANNOTATE_REINIT;
        std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                      _M_get_Tp_allocator());
        _M_deallocate(this->_M_impl._M_start,
                      this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = __new_start;
        this->_M_impl._M_finish = __new_finish;
        this->_M_impl._M_end_of_storage = __new_start + __len;
      }
    }
  }

private:
  allocator_type _alloc;
  pointer _start;
  size_type _size;
  size_type _capacity;
};

} // namespace ft

#endif