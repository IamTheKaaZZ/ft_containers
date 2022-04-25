/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/04/25 17:55:03 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "AllocatorTraits.hpp"
#include "Iterators.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
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
  typedef Allocator allocator_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef ft::random_access_iterator<vector> iterator;
  typedef ft::const_random_access_iterator<vector> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  static_assert(
      (ft::is_same<typename allocator_type::value_type, value_type>::value),
      "Allocator::value_type must be same type as value_type");
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
    AllocTraits::on_swap(getTAllocator(), other.getTAllocator());
  }
  void clear() { eraseAtEnd(this->implV.start); }

protected:
  using Base::allocate;
  using Base::deallocate;
  using Base::getTAllocator;
  using Base::implV;
  void rangeCheck(size_type n) const {
    static_assert(n >= this->size(), "Invalid Range");
  }
  void requireNonEmpty() { static_assert(!empty(), "Vector is empty"); }
  template <typename ForwardIterator>
  pointer allocateAndCopy(size_type n, ForwardIterator first,
                          ForwardIterator last) {
    pointer result = this->allocate(n);
    try {
      std::uninitialized_copy(first, last, result);
      return result;
    } catch (...) {
      deallocate(result, n);
      throw;
    }
  }
  template <typename Integer>
  void initializeDispatch(Integer n, Integer value, ft::true_type) {
    this->implV.start = allocate(static_cast<size_type>(n));
    this->implV.endOfStorage = this->implV.start + static_cast<size_type>(n);
    fillInitialize(static_cast<size_type>(n), value);
  }
  template <typename InputIterator>
  void initializeDispatch(InputIterator first, InputIterator last,
                          ft::false_type) {
    typedef typename std::iterator_traits<InputIterator>::iterator_category
        IterCategory;
    rangeInitialize(first, last, IterCategory());
  }
  template <typename InputIterator>
  void rangeInitialize(InputIterator first, InputIterator last,
                       ft::input_iterator_tag) {
    for (; first != last; ++first)
      push_back(*first);
  }
  template <typename ForwardIterator>
  void rangeInitialize(ForwardIterator first, ForwardIterator last,
                       ft::forward_iterator_tag) {
    const size_type n = ft::distance(first, last);
    this->implV.start = this->_M_allocate(n);
    this->implV.endOfStorage = this->implV.start + n;
    this->implV.finish =
        std::uninitialized_copy(first, last, this->implV.start);
  }
  void fillInitialize(size_type n, const value_type &value) {
    std::uninitialized_fill_n(this->implV.start, n, value);
  }
  template <typename Integer>
  void assignDispatch(Integer n, Integer val, ft::true_type) {
    fillAssign(n, val);
  }
  template <typename InputIterator>
  void assignDispatch(InputIterator first, InputIterator last, ft::false_type) {
    assignAux(first, last, ft::__iterator_category(first));
  }
  template <typename InputIterator>
  void assignAux(InputIterator first, InputIterator last,
                 ft::input_iterator_tag);
  template <typename ForwardIterator>
  void assignAux(ForwardIterator first, ForwardIterator last,
                 ft::forward_iterator_tag);
  void fillAssign(size_type n, const value_type &val);
  template <typename Integer>
  void insertDispatch(iterator pos, Integer n, Integer val, ft::true_type) {
    fillInsert(pos, n, val);
  }
  template <typename InputIterator>
  void insertDispatch(iterator pos, InputIterator first, InputIterator last,
                      ft::false_type) {
    rangeInsert(pos, first, last, ft::__iterator_category(first));
  }
  template <typename InputIterator>
  void rangeInsert(iterator pos, InputIterator first, InputIterator last,
                   ft::input_iterator_tag);
  template <typename ForwardIterator>
  void rangeInsert(iterator pos, ForwardIterator first, ForwardIterator last,
                   ft::forward_iterator_tag);
  void fillInsert(iterator pos, size_type n, const value_type &x);
  void insertAux(iterator position, const value_type &x);
  void reallocInsert(iterator position, const value_type &x);
  size_type checkLen(size_type n, const char *s) const {
    if (max_size() - size() < n)
      throw std::length_error(s);
    const size_type len = size() + std::max(size(), n);
    return (len < size() || len > max_size()) ? max_size() : len;
  }
  void eraseAtEnd(pointer pos) {
    if (size_type n = this->implV.finish - pos) {
      AllocTraits::destroy(getTAllocator(), pos);
      this->implV.finish = pos;
    }
  }
  iterator eraseV(iterator position) {
    if (position + 1 != end())
      _GLIBCXX_MOVE3(position + 1, end(), position);
    --this->implV.finish;
    AllocTraits::destroy(this->_M_impl, this->_M_impl._M_finish);
    return position;
  }
  iterator eraseV(iterator first, iterator last) {
    if (first != last) {
      if (last != end())
        _GLIBCXX_MOVE3(last, end(), first);
      eraseAtEnd(first.base() + (end() - last));
    }
    return first;
  }
  template <typename U> U *dataPtr(U *ptr) const { return ptr; }
  template <typename U> U *dataPtr(U *ptr) { return ptr; }
  template <typename Ptr> value_type *dataPtr(Ptr ptr) {
    return empty() ? (value_type *)0 : ptr.operator->();
  }
  template <typename Ptr> const value_type *dataPtr(Ptr ptr) const {
    return empty() ? (const value_type *)0 : ptr.operator->();
  }
  template<typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>&
    vector<_Tp, _Alloc>::
    operator=(const vector<_Tp, _Alloc>& __x)
    {
      if (&__x != this)
        {
          _GLIBCXX_ASAN_ANNOTATE_REINIT;
#if __cplusplus >= 201103L
          if (_Alloc_traits::_S_propagate_on_copy_assign())
            {
              if (!_Alloc_traits::_S_always_equal()
                  && _M_get_Tp_allocator() != __x._M_get_Tp_allocator())
                {
                  // replacement allocator cannot free existing storage
                  this->clear();
                  _M_deallocate(this->_M_impl._M_start,
                                this->_M_impl._M_end_of_storage
                                - this->_M_impl._M_start);
                  this->_M_impl._M_start = nullptr;
                  this->_M_impl._M_finish = nullptr;
                  this->_M_impl._M_end_of_storage = nullptr;
                }
              std::__alloc_on_copy(_M_get_Tp_allocator(),
                                   __x._M_get_Tp_allocator());
            }
#endif
          const size_type __xlen = __x.size();
          if (__xlen > capacity())
            {
              pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(),
                                                   __x.end());
              std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                            _M_get_Tp_allocator());
              _M_deallocate(this->_M_impl._M_start,
                            this->_M_impl._M_end_of_storage
                            - this->_M_impl._M_start);
              this->_M_impl._M_start = __tmp;
              this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __xlen;
            }
          else if (size() >= __xlen)
            {
              std::_Destroy(std::copy(__x.begin(), __x.end(), begin()),
                            end(), _M_get_Tp_allocator());
            }
          else
            {
              std::copy(__x._M_impl._M_start, __x._M_impl._M_start + size(),
                        this->_M_impl._M_start);
              std::__uninitialized_copy_a(__x._M_impl._M_start + size(),
                                          __x._M_impl._M_finish,
                                          this->_M_impl._M_finish,
                                          _M_get_Tp_allocator());
            }
          this->_M_impl._M_finish = this->_M_impl._M_start + __xlen;
        }
      return *this;
    }
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp, _Alloc>::
    _M_fill_assign(size_t __n, const value_type& __val)
    {
      if (__n > capacity())
        {
          vector __tmp(__n, __val, _M_get_Tp_allocator());
          __tmp._M_impl._M_swap_data(this->_M_impl);
        }
      else if (__n > size())
        {
          std::fill(begin(), end(), __val);
          const size_type __add = __n - size();
          _GLIBCXX_ASAN_ANNOTATE_GROW(__add);
          this->_M_impl._M_finish =
            std::__uninitialized_fill_n_a(this->_M_impl._M_finish,
                                          __add, __val, _M_get_Tp_allocator());
          _GLIBCXX_ASAN_ANNOTATE_GREW(__add);
        }
      else
        _M_erase_at_end(std::fill_n(this->_M_impl._M_start, __n, __val));
    }
  template<typename _Tp, typename _Alloc>
    template<typename _InputIterator>
      void
      vector<_Tp, _Alloc>::
      _M_assign_aux(_InputIterator __first, _InputIterator __last,
                    std::input_iterator_tag)
      {
        pointer __cur(this->_M_impl._M_start);
        for (; __first != __last && __cur != this->_M_impl._M_finish;
             ++__cur, ++__first)
          *__cur = *__first;
        if (__first == __last)
          _M_erase_at_end(__cur);
        else
          _M_range_insert(end(), __first, __last,
                          std::__iterator_category(__first));
      }
  template<typename _Tp, typename _Alloc>
    template<typename _ForwardIterator>
      void
      vector<_Tp, _Alloc>::
      _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                    std::forward_iterator_tag)
      {
        const size_type __len = std::distance(__first, __last);
        if (__len > capacity())
          {
            pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
            _GLIBCXX_ASAN_ANNOTATE_REINIT;
            std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                          _M_get_Tp_allocator());
            _M_deallocate(this->_M_impl._M_start,
                          this->_M_impl._M_end_of_storage
                          - this->_M_impl._M_start);
            this->_M_impl._M_start = __tmp;
            this->_M_impl._M_finish = this->_M_impl._M_start + __len;
            this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
          }
        else if (size() >= __len)
          _M_erase_at_end(std::copy(__first, __last, this->_M_impl._M_start));
        else
          {
            _ForwardIterator __mid = __first;
            std::advance(__mid, size());
            std::copy(__first, __mid, this->_M_impl._M_start);
            const size_type __attribute__((__unused__)) __n = __len - size();
            _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
            this->_M_impl._M_finish =
              std::__uninitialized_copy_a(__mid, __last,
                                          this->_M_impl._M_finish,
                                          _M_get_Tp_allocator());
            _GLIBCXX_ASAN_ANNOTATE_GREW(__n);
          }
      }
#if __cplusplus >= 201103L
  template<typename _Tp, typename _Alloc>
    auto
    vector<_Tp, _Alloc>::
    _M_insert_rval(const_iterator __position, value_type&& __v) -> iterator
    {
      const auto __n = __position - cbegin();
      if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
        if (__position == cend())
          {
            _GLIBCXX_ASAN_ANNOTATE_GROW(1);
            _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                                     std::move(__v));
            ++this->_M_impl._M_finish;
            _GLIBCXX_ASAN_ANNOTATE_GREW(1);
          }
        else
          _M_insert_aux(begin() + __n, std::move(__v));
      else
        _M_realloc_insert(begin() + __n, std::move(__v));
      return iterator(this->_M_impl._M_start + __n);
    }
  template<typename _Tp, typename _Alloc>
    template<typename... _Args>
      auto
      vector<_Tp, _Alloc>::
      _M_emplace_aux(const_iterator __position, _Args&&... __args)
      -> iterator
      {
        const auto __n = __position - cbegin();
        if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
          if (__position == cend())
            {
              _GLIBCXX_ASAN_ANNOTATE_GROW(1);
              _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                                       std::forward<_Args>(__args)...);
              ++this->_M_impl._M_finish;
              _GLIBCXX_ASAN_ANNOTATE_GREW(1);
            }
          else
            {
              // We need to construct a temporary because something in __args...
              // could alias one of the elements of the container and so we
              // need to use it before _M_insert_aux moves elements around.
              _Temporary_value __tmp(this, std::forward<_Args>(__args)...);
              _M_insert_aux(begin() + __n, std::move(__tmp._M_val()));
            }
        else
          _M_realloc_insert(begin() + __n, std::forward<_Args>(__args)...);
        return iterator(this->_M_impl._M_start + __n);
      }
  template<typename _Tp, typename _Alloc>
    template<typename _Arg>
      void
      vector<_Tp, _Alloc>::
      _M_insert_aux(iterator __position, _Arg&& __arg)
#else
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp, _Alloc>::
    _M_insert_aux(iterator __position, const _Tp& __x)
#endif
    {
      _GLIBCXX_ASAN_ANNOTATE_GROW(1);
      _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                               _GLIBCXX_MOVE(*(this->_M_impl._M_finish - 1)));
      ++this->_M_impl._M_finish;
      _GLIBCXX_ASAN_ANNOTATE_GREW(1);
#if __cplusplus < 201103L
      _Tp __x_copy = __x;
#endif
      _GLIBCXX_MOVE_BACKWARD3(__position.base(),
                              this->_M_impl._M_finish - 2,
                              this->_M_impl._M_finish - 1);
#if __cplusplus < 201103L
      *__position = __x_copy;
#else
      *__position = std::forward<_Arg>(__arg);
#endif
    }
#if __cplusplus >= 201103L
  template<typename _Tp, typename _Alloc>
    template<typename... _Args>
      void
      vector<_Tp, _Alloc>::
      _M_realloc_insert(iterator __position, _Args&&... __args)
#else
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp, _Alloc>::
    _M_realloc_insert(iterator __position, const _Tp& __x)
#endif
    {
      const size_type __len =
        _M_check_len(size_type(1), "vector::_M_realloc_insert");
      pointer __old_start = this->_M_impl._M_start;
      pointer __old_finish = this->_M_impl._M_finish;
      const size_type __elems_before = __position - begin();
      pointer __new_start(this->_M_allocate(__len));
      pointer __new_finish(__new_start);
      __try
        {
          // The order of the three operations is dictated by the C++11
          // case, where the moves could alter a new element belonging
          // to the existing vector.  This is an issue only for callers
          // taking the element by lvalue ref (see last bullet of C++11
          // [res.on.arguments]).
          _Alloc_traits::construct(this->_M_impl,
                                   __new_start + __elems_before,
#if __cplusplus >= 201103L
                                   std::forward<_Args>(__args)...);
#else
                                   __x);
#endif
          __new_finish = pointer();
          __new_finish
            = std::__uninitialized_move_if_noexcept_a
            (__old_start, __position.base(),
             __new_start, _M_get_Tp_allocator());
          ++__new_finish;
          __new_finish
            = std::__uninitialized_move_if_noexcept_a
            (__position.base(), __old_finish,
             __new_finish, _M_get_Tp_allocator());
        }
      __catch(...)
        {
          if (!__new_finish)
            _Alloc_traits::destroy(this->_M_impl,
                                   __new_start + __elems_before);
          else
            std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
          _M_deallocate(__new_start, __len);
          __throw_exception_again;
        }
      _GLIBCXX_ASAN_ANNOTATE_REINIT;
      std::_Destroy(__old_start, __old_finish, _M_get_Tp_allocator());
      _M_deallocate(__old_start,
                    this->_M_impl._M_end_of_storage - __old_start);
      this->_M_impl._M_start = __new_start;
      this->_M_impl._M_finish = __new_finish;
      this->_M_impl._M_end_of_storage = __new_start + __len;
    }
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp, _Alloc>::
    _M_fill_insert(iterator __position, size_type __n, const value_type& __x)
    {
      if (__n != 0)
        {
          if (size_type(this->_M_impl._M_end_of_storage
                        - this->_M_impl._M_finish) >= __n)
            {
#if __cplusplus < 201103L
              value_type __x_copy = __x;
#else
              _Temporary_value __tmp(this, __x);
              value_type& __x_copy = __tmp._M_val();
#endif
              const size_type __elems_after = end() - __position;
              pointer __old_finish(this->_M_impl._M_finish);
              if (__elems_after > __n)
                {
                  _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
                  std::__uninitialized_move_a(this->_M_impl._M_finish - __n,
                                              this->_M_impl._M_finish,
                                              this->_M_impl._M_finish,
                                              _M_get_Tp_allocator());
                  this->_M_impl._M_finish += __n;
                  _GLIBCXX_ASAN_ANNOTATE_GREW(__n);
                  _GLIBCXX_MOVE_BACKWARD3(__position.base(),
                                          __old_finish - __n, __old_finish);
                  std::fill(__position.base(), __position.base() + __n,
                            __x_copy);
                }
              else
                {
                  _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
                  this->_M_impl._M_finish =
                    std::__uninitialized_fill_n_a(this->_M_impl._M_finish,
                                                  __n - __elems_after,
                                                  __x_copy,
                                                  _M_get_Tp_allocator());
                  _GLIBCXX_ASAN_ANNOTATE_GREW(__n - __elems_after);
                  std::__uninitialized_move_a(__position.base(), __old_finish,
                                              this->_M_impl._M_finish,
                                              _M_get_Tp_allocator());
                  this->_M_impl._M_finish += __elems_after;
                  _GLIBCXX_ASAN_ANNOTATE_GREW(__elems_after);
                  std::fill(__position.base(), __old_finish, __x_copy);
                }
            }
          else
            {
              const size_type __len =
                _M_check_len(__n, "vector::_M_fill_insert");
              const size_type __elems_before = __position - begin();
              pointer __new_start(this->_M_allocate(__len));
              pointer __new_finish(__new_start);
              __try
                {
                  // See _M_realloc_insert above.
                  std::__uninitialized_fill_n_a(__new_start + __elems_before,
                                                __n, __x,
                                                _M_get_Tp_allocator());
                  __new_finish = pointer();
                  __new_finish
                    = std::__uninitialized_move_if_noexcept_a
                    (this->_M_impl._M_start, __position.base(),
                     __new_start, _M_get_Tp_allocator());
                  __new_finish += __n;
                  __new_finish
                    = std::__uninitialized_move_if_noexcept_a
                    (__position.base(), this->_M_impl._M_finish,
                     __new_finish, _M_get_Tp_allocator());
                }
              __catch(...)
                {
                  if (!__new_finish)
                    std::_Destroy(__new_start + __elems_before,
                                  __new_start + __elems_before + __n,
                                  _M_get_Tp_allocator());
                  else
                    std::_Destroy(__new_start, __new_finish,
                                  _M_get_Tp_allocator());
                  _M_deallocate(__new_start, __len);
                  __throw_exception_again;
                }
              _GLIBCXX_ASAN_ANNOTATE_REINIT;
              std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                            _M_get_Tp_allocator());
              _M_deallocate(this->_M_impl._M_start,
                            this->_M_impl._M_end_of_storage
                            - this->_M_impl._M_start);
              this->_M_impl._M_start = __new_start;
              this->_M_impl._M_finish = __new_finish;
              this->_M_impl._M_end_of_storage = __new_start + __len;
            }
        }
    }
#if __cplusplus >= 201103L
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp, _Alloc>::
    _M_default_append(size_type __n)
    {
      if (__n != 0)
        {
          if (size_type(this->_M_impl._M_end_of_storage
                        - this->_M_impl._M_finish) >= __n)
            {
              _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
              this->_M_impl._M_finish =
                std::__uninitialized_default_n_a(this->_M_impl._M_finish,
                                                 __n, _M_get_Tp_allocator());
              _GLIBCXX_ASAN_ANNOTATE_GREW(__n);
            }
          else
            {
              const size_type __len =
                _M_check_len(__n, "vector::_M_default_append");
              const size_type __old_size = this->size();
              pointer __new_start(this->_M_allocate(__len));
              pointer __new_finish(__new_start);
              __try
                {
                  __new_finish
                    = std::__uninitialized_move_if_noexcept_a
                    (this->_M_impl._M_start, this->_M_impl._M_finish,
                     __new_start, _M_get_Tp_allocator());
                  __new_finish =
                    std::__uninitialized_default_n_a(__new_finish, __n,
                                                     _M_get_Tp_allocator());
                }
              __catch(...)
                {
                  std::_Destroy(__new_start, __new_finish,
                                _M_get_Tp_allocator());
                  _M_deallocate(__new_start, __len);
                  __throw_exception_again;
                }
              _GLIBCXX_ASAN_ANNOTATE_REINIT;
              std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                            _M_get_Tp_allocator());
              _M_deallocate(this->_M_impl._M_start,
                            this->_M_impl._M_end_of_storage
                            - this->_M_impl._M_start);
              this->_M_impl._M_start = __new_start;
              this->_M_impl._M_finish = __new_finish;
              this->_M_impl._M_end_of_storage = __new_start + __len;
            }
        }
    }
  template<typename _Tp, typename _Alloc>
    bool
    vector<_Tp, _Alloc>::
    _M_shrink_to_fit()
    {
      if (capacity() == size())
        return false;
      _GLIBCXX_ASAN_ANNOTATE_REINIT;
      return std::__shrink_to_fit_aux<vector>::_S_do_it(*this);
    }
#endif
  template<typename _Tp, typename _Alloc>
    template<typename _InputIterator>
      void
      vector<_Tp, _Alloc>::
      _M_range_insert(iterator __pos, _InputIterator __first,
                      _InputIterator __last, std::input_iterator_tag)
      {
        if (__pos == end())
          {
            for (; __first != __last; ++__first)
              insert(end(), *__first);
          }
        else if (__first != __last)
          {
            vector __tmp(__first, __last, _M_get_Tp_allocator());
            insert(__pos,
                   _GLIBCXX_MAKE_MOVE_ITERATOR(__tmp.begin()),
                   _GLIBCXX_MAKE_MOVE_ITERATOR(__tmp.end()));
          }
      }
  template<typename _Tp, typename _Alloc>
    template<typename _ForwardIterator>
      void
      vector<_Tp, _Alloc>::
      _M_range_insert(iterator __position, _ForwardIterator __first,
                      _ForwardIterator __last, std::forward_iterator_tag)
      {
        if (__first != __last)
          {
            const size_type __n = std::distance(__first, __last);
            if (size_type(this->_M_impl._M_end_of_storage
                          - this->_M_impl._M_finish) >= __n)
              {
                const size_type __elems_after = end() - __position;
                pointer __old_finish(this->_M_impl._M_finish);
                if (__elems_after > __n)
                  {
                    _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
                    std::__uninitialized_move_a(this->_M_impl._M_finish - __n,
                                                this->_M_impl._M_finish,
                                                this->_M_impl._M_finish,
                                                _M_get_Tp_allocator());
                    this->_M_impl._M_finish += __n;
                    _GLIBCXX_ASAN_ANNOTATE_GREW(__n);
                    _GLIBCXX_MOVE_BACKWARD3(__position.base(),
                                            __old_finish - __n, __old_finish);
                    std::copy(__first, __last, __position);
                  }
                else
                  {
                    _ForwardIterator __mid = __first;
                    std::advance(__mid, __elems_after);
                    _GLIBCXX_ASAN_ANNOTATE_GROW(__n);
                    std::__uninitialized_copy_a(__mid, __last,
                                                this->_M_impl._M_finish,
                                                _M_get_Tp_allocator());
                    this->_M_impl._M_finish += __n - __elems_after;
                    _GLIBCXX_ASAN_ANNOTATE_GREW(__n - __elems_after);
                    std::__uninitialized_move_a(__position.base(),
                                                __old_finish,
                                                this->_M_impl._M_finish,
                                                _M_get_Tp_allocator());
                    this->_M_impl._M_finish += __elems_after;
                    _GLIBCXX_ASAN_ANNOTATE_GREW(__elems_after);
                    std::copy(__first, __mid, __position);
                  }
              }
            else
              {
                const size_type __len =
                  _M_check_len(__n, "vector::_M_range_insert");
                pointer __new_start(this->_M_allocate(__len));
                pointer __new_finish(__new_start);
                __try
                  {
                    __new_finish
                      = std::__uninitialized_move_if_noexcept_a
                      (this->_M_impl._M_start, __position.base(),
                       __new_start, _M_get_Tp_allocator());
                    __new_finish
                      = std::__uninitialized_copy_a(__first, __last,
                                                    __new_finish,
                                                    _M_get_Tp_allocator());
                    __new_finish
                      = std::__uninitialized_move_if_noexcept_a
                      (__position.base(), this->_M_impl._M_finish,
                       __new_finish, _M_get_Tp_allocator());
                  }
                __catch(...)
                  {
                    std::_Destroy(__new_start, __new_finish,
                                  _M_get_Tp_allocator());
                    _M_deallocate(__new_start, __len);
                    __throw_exception_again;
                  }
                _GLIBCXX_ASAN_ANNOTATE_REINIT;
                std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
                              _M_get_Tp_allocator());
                _M_deallocate(this->_M_impl._M_start,
                              this->_M_impl._M_end_of_storage
                              - this->_M_impl._M_start);
                this->_M_impl._M_start = __new_start;
                this->_M_impl._M_finish = __new_finish;
                this->_M_impl._M_end_of_storage = __new_start + __len;
              }
          }
      }
  // vector<bool>
  template<typename _Alloc>
    void
    vector<bool, _Alloc>::
    _M_reallocate(size_type __n)
    {
      _Bit_pointer __q = this->_M_allocate(__n);
      iterator __start(std::__addressof(*__q), 0);
      iterator __finish(_M_copy_aligned(begin(), end(), __start));
      this->_M_deallocate();
      this->_M_impl._M_start = __start;
      this->_M_impl._M_finish = __finish;
      this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
    }
  template<typename _Alloc>
    void
    vector<bool, _Alloc>::
    _M_fill_insert(iterator __position, size_type __n, bool __x)
    {
      if (__n == 0)
        return;
      if (capacity() - size() >= __n)
        {
          std::copy_backward(__position, end(),
                             this->_M_impl._M_finish + difference_type(__n));
          std::fill(__position, __position + difference_type(__n), __x);
          this->_M_impl._M_finish += difference_type(__n);
        }
      else
        {
          const size_type __len = 
            _M_check_len(__n, "vector<bool>::_M_fill_insert");
          _Bit_pointer __q = this->_M_allocate(__len);
          iterator __start(std::__addressof(*__q), 0);
          iterator __i = _M_copy_aligned(begin(), __position, __start);
          std::fill(__i, __i + difference_type(__n), __x);
          iterator __finish = std::copy(__position, end(),
                                        __i + difference_type(__n));
          this->_M_deallocate();
          this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
          this->_M_impl._M_start = __start;
          this->_M_impl._M_finish = __finish;
        }
    }
  template<typename _Alloc>
    template<typename _ForwardIterator>
      void
      vector<bool, _Alloc>::
      _M_insert_range(iterator __position, _ForwardIterator __first, 
                      _ForwardIterator __last, std::forward_iterator_tag)
      {
        if (__first != __last)
          {
            size_type __n = std::distance(__first, __last);
            if (capacity() - size() >= __n)
              {
                std::copy_backward(__position, end(),
                                   this->_M_impl._M_finish
                                   + difference_type(__n));
                std::copy(__first, __last, __position);
                this->_M_impl._M_finish += difference_type(__n);
              }
            else
              {
                const size_type __len =
                  _M_check_len(__n, "vector<bool>::_M_insert_range");
                _Bit_pointer __q = this->_M_allocate(__len);
                iterator __start(std::__addressof(*__q), 0);
                iterator __i = _M_copy_aligned(begin(), __position, __start);
                __i = std::copy(__first, __last, __i);
                iterator __finish = std::copy(__position, end(), __i);
                this->_M_deallocate();
                this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
                this->_M_impl._M_start = __start;
                this->_M_impl._M_finish = __finish;
              }
          }
      }
  template<typename _Alloc>
    void
    vector<bool, _Alloc>::
    _M_insert_aux(iterator __position, bool __x)
    {
      if (this->_M_impl._M_finish._M_p != this->_M_impl._M_end_addr())
        {
          std::copy_backward(__position, this->_M_impl._M_finish, 
                             this->_M_impl._M_finish + 1);
          *__position = __x;
          ++this->_M_impl._M_finish;
        }
      else
        {
          const size_type __len =
            _M_check_len(size_type(1), "vector<bool>::_M_insert_aux");
          _Bit_pointer __q = this->_M_allocate(__len);
          iterator __start(std::__addressof(*__q), 0);
          iterator __i = _M_copy_aligned(begin(), __position, __start);
          *__i++ = __x;
          iterator __finish = std::copy(__position, end(), __i);
          this->_M_deallocate();
          this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
          this->_M_impl._M_start = __start;
          this->_M_impl._M_finish = __finish;
        }
    }
  template<typename _Alloc>
    typename vector<bool, _Alloc>::iterator
    vector<bool, _Alloc>::
    _M_erase(iterator __position)
    {
      if (__position + 1 != end())
        std::copy(__position + 1, end(), __position);
      --this->_M_impl._M_finish;
      return __position;
    }
  template<typename _Alloc>
    typename vector<bool, _Alloc>::iterator
    vector<bool, _Alloc>::
    _M_erase(iterator __first, iterator __last)
    {
      if (__first != __last)
        _M_erase_at_end(std::copy(__last, end(), __first));
      return __first;
    }
#if __cplusplus >= 201103L
  template<typename _Alloc>
    bool
    vector<bool, _Alloc>::
    _M_shrink_to_fit()
    {
      if (capacity() - size() < int(_S_word_bit))
        return false;
      __try
        {
          _M_reallocate(size());
          return true;
        }
      __catch(...)
        { return false; }
    }
#endif

private:
  size_type _size;
  size_type _nElements;
};

} // namespace ft

#endif