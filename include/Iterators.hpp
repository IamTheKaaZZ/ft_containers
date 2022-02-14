/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Iterators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 17:13:16 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/14 12:31:43 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include <cstddef>
// # include <iterator>
#include "utility.hpp"

namespace ft {

//  Iterator_tags //
//---------------------------------------//
struct input_iterator_tag {
  typedef input_iterator_tag tags;
  char const tag = 'i';
  friend bool operator==(tags const &lhs, tags &rhs) {
    return (lhs.tag == rhs.tag);
  };
};
struct output_iterator_tag {
  typedef output_iterator_tag tags;
  char const tag = 'o';
  friend bool operator==(tags const &lhs, tags &rhs) {
    return (lhs.tag == rhs.tag);
  };
};
struct forward_iterator_tag : public input_iterator_tag {
  typedef forward_iterator_tag tags;
  char const tag = 'f';
  friend bool operator==(tags const &lhs, tags &rhs) {
    return (lhs.tag == rhs.tag);
  };
};
struct bidirectional_iterator_tag : public forward_iterator_tag {
  typedef bidirectional_iterator_tag tags;
  char const tag = 'b';
  friend bool operator==(tags const &lhs, tags &rhs) {
    return (lhs.tag == rhs.tag);
  };
};
struct random_access_iterator_tag : public bidirectional_iterator_tag {
  typedef random_access_iterator_tag tags;
  char const tag = 'r';
  friend bool operator==(tags const &lhs, tags &rhs) {
    return (lhs.tag == rhs.tag);
  };
};

//  Iterator_traits //
//---------------------------------------//
template <class Iter> struct iterator_traits {
  typedef typename Iter::difference_type difference_type;
  typedef typename Iter::value_type value_type;
  typedef typename Iter::pointer pointer;
  typedef typename Iter::reference reference;
  typedef typename Iter::iterator_category iterator_category;
};

//  T* specialization
template <class T> struct iterator_traits<T *> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef random_access_iterator_tag iterator_category;
};

//  const T* specialization
template <class T> struct iterator_traits<const T *> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef const T *pointer;
  typedef const T &reference;
  typedef random_access_iterator_tag iterator_category;
};

//  Base Iterator and Const Iterator //
//---------------------------------------//
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T *, class Reference = T &>
class iterator {

public:
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef Category iterator_category;

  iterator() = delete;
  explicit iterator(pointer ptr) : _ptr(ptr){};
  iterator(iterator const &) = default;
  virtual ~iterator() = default;
  iterator &operator=(const iterator &rhs) = default;

  // increment/decrement
  virtual iterator &operator++() {
    _ptr++;
    return *this;
  }
  virtual iterator operator++(int) {
    iterator tmp(*this);
    _ptr++;
    return tmp;
  }
  // virtual iterator&   operator--() = 0;
  // virtual iterator    operator--(int) = 0;
  // arithmatic
  // virtual iterator&		operator+=(difference_type) = 0;
  // virtual iterator		operator+(difference_type) const = 0;
  // virtual friend iterator	operator+(difference_type, iterator const &) =
  // 0; virtual iterator&		operator-=(difference_type) = 0; virtual
  // iterator		operator-(difference_type) const = 0; virtual
  // difference_type	operator-(iterator) const = 0;
  // boolean
  // virtual friend bool    operator==(iterator const &, iterator const &) = 0;
  // virtual friend bool    operator!=(iterator const &, iterator const &) = 0;
  // virtual friend bool    operator<(iterator const &, iterator const &) const
  // = 0; virtual friend bool    operator>(iterator const &, iterator const &)
  // const = 0; virtual friend bool    operator<=(iterator const &, iterator
  // const &) const = 0; virtual friend bool    operator>=(iterator const &,
  // iterator const &) const = 0;
  // access
  virtual reference operator*() const { return *(this->_ptr); }
  // virtual pointer     operator->() const = 0;
  // virtual reference   operator[](difference_type) const = 0;

protected:
  pointer _ptr;
};

template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T *, class Reference = T &>
class const_iterator {

public:
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer const pointer;
  typedef Reference const reference;
  typedef Category iterator_category;

  const_iterator() = delete;
  explicit const_iterator(pointer ptr) : _ptr(ptr){};
  const_iterator(const_iterator const &) = default;
  const_iterator(iterator<Category, T> const &src) : _ptr(src._ptr){};
  virtual ~const_iterator() = default;
  const_iterator &operator=(const const_iterator &rhs) = default;

  // increment/decrement
  virtual const_iterator &operator++() {
    _ptr++;
    return *this;
  }
  virtual const_iterator operator++(int) {
    const_iterator tmp(*this);
    _ptr++;
    return tmp;
  }
  // virtual const_iterator&   operator--() = 0;
  // virtual const_iterator    operator--(int) = 0;
  // arithmatic
  // virtual const_iterator&			operator+=(difference_type) = 0;
  // virtual const_iterator			operator+(difference_type) const =
  // 0; virtual friend const_iterator	operator+(difference_type,
  // const_iterator const &) = 0; virtual const_iterator&
  // operator-=(difference_type) = 0; virtual const_iterator
  // operator-(difference_type) const = 0; virtual difference_type
  // operator-(const_iterator) const = 0;
  // boolean
  // virtual friend bool    operator==(const_iterator const &, const_iterator
  // const &) = 0; virtual friend bool    operator!=(const_iterator const &,
  // const_iterator const &) = 0; virtual friend bool operator<(const_iterator
  // const &, const_iterator const &) const = 0; virtual friend bool
  // operator>(const_iterator const &, const_iterator const &) const = 0;
  // virtual friend bool    operator<=(const_iterator const &, const_iterator
  // const &) const = 0; virtual friend bool    operator>=(const_iterator const
  // &, const_iterator const &) const = 0;
  // access
  virtual reference operator*() const { return *(this->_ptr); }
  // virtual pointer     operator->() const = 0;
  // virtual reference   operator[](difference_type) const = 0;

protected:
  pointer _ptr;
};

//  Input (Const) Iterator //
//---------------------------------------//
template <class T>
class input_iterator : virtual public iterator<input_iterator_tag, T> {

public:
  typedef iterator<input_iterator_tag, T> It;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  input_iterator &operator++() {
    It::operator++();
    return *this;
  }
  input_iterator operator++(int) {
    input_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(input_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(input_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return this->_ptr; }
};

template <class T>
class const_input_iterator
    : virtual public const_iterator<input_iterator_tag, T> {

public:
  typedef const_iterator<input_iterator_tag, T> It;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  const_input_iterator &operator++() {
    It::operator++();
    return *this;
  }
  const_input_iterator operator++(int) {
    const_input_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(const_input_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(const_input_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return this->_ptr; }
};

//  Output (Const) Iterator //
//---------------------------------------//
template <class T>
class output_iterator : virtual public iterator<output_iterator_tag, T> {

public:
  typedef iterator<output_iterator_tag, T> It;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  output_iterator &operator++() {
    It::operator++();
    return *this;
  }
  output_iterator operator++(int) {
    output_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual reference operator*() const { return *(this->_ptr); }
};
template <class T>
class const_output_iterator
    : virtual public const_iterator<output_iterator_tag, T> {

public:
  typedef const_iterator<output_iterator_tag, T> It;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  const_output_iterator &operator++() {
    It::operator++();
    return *this;
  }
  const_output_iterator operator++(int) {
    const_output_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual reference operator*() const { return *(this->_ptr); }
};

//  Forward (Const) Iterator //
//---------------------------------------//
template <class T>
class forward_iterator : virtual public input_iterator<T>,
                         virtual public output_iterator<T>,
                         virtual public iterator<forward_iterator_tag, T> {

public:
  typedef iterator<forward_iterator_tag, T> It;
  typedef input_iterator<T> inIt;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  forward_iterator() : input_iterator<T>(), output_iterator<T>(), It() {
    this->_ptr(nullptr);
  }

  forward_iterator<T> &operator++() {
    It::operator++();
    return *this;
  }
  forward_iterator<T> operator++(int) {
    forward_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(forward_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(forward_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return inIt::operator->(); }
};
template <class T>
class const_forward_iterator
    : virtual public const_input_iterator<T>,
      virtual public const_iterator<forward_iterator_tag, T> {

public:
  typedef const_iterator<forward_iterator_tag, T> It;
  typedef const_input_iterator<T> inIt;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  const_forward_iterator() : const_input_iterator<T>(), It() {
    this->_ptr(nullptr);
  }

  const_forward_iterator<T> &operator++() {
    using It::operator++;
    return *this;
  }
  const_forward_iterator<T> operator++(int) {
    It tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(forward_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(forward_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return inIt::operator->(); }
};

//  Bidirectional (Const) Iterator //
//---------------------------------------//
template <class T>
class bidirectional_iterator
    : virtual public forward_iterator<T>,
      virtual public iterator<bidirectional_iterator_tag, T> {

public:
  typedef iterator<bidirectional_iterator_tag, T> It;
  typedef forward_iterator<T> fIt;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  bidirectional_iterator() : forward_iterator<T>(), It() {}

  bidirectional_iterator &operator++() {
    It::operator++();
    return *this;
  }
  bidirectional_iterator operator++(int) {
    bidirectional_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(bidirectional_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(bidirectional_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return fIt::operator->(); }
  bidirectional_iterator<T> &operator--() {
    this->_ptr--;
    return *this;
  };
  bidirectional_iterator<T> operator--(int) {
    bidirectional_iterator<T> tmp(*this);
    --*this;
    return tmp;
  };
};
template <class T>
class const_bidirectional_iterator
    : virtual public const_forward_iterator<T>,
      virtual public const_iterator<bidirectional_iterator_tag, T> {

public:
  typedef iterator<bidirectional_iterator_tag, T> It;
  typedef const_forward_iterator<T> fIt;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  const_bidirectional_iterator() : forward_iterator<T>(), It() {}

  const_bidirectional_iterator &operator++() {
    It::operator++();
    return *this;
  }
  const_bidirectional_iterator operator++(int) {
    const_bidirectional_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(const_bidirectional_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(const_bidirectional_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return fIt::operator->(); }
  const_bidirectional_iterator<T> &operator--() {
    this->_ptr--;
    return *this;
  };
  const_bidirectional_iterator<T> operator--(int) {
    const_bidirectional_iterator<T> tmp(*this);
    --*this;
    return tmp;
  };
};

//  Random Access (Const) Iterator //
//---------------------------------------//
template <class T>
class random_access_iterator
    : virtual public bidirectional_iterator<T>,
      virtual public iterator<random_access_iterator_tag, T> {

public:
  typedef iterator<random_access_iterator_tag, T> It;
  typedef bidirectional_iterator<T> bIt;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  random_access_iterator<T>() : bidirectional_iterator<T>(), It() {}

  random_access_iterator<T> &operator++() {
    It::operator++();
    return *this;
  }
  random_access_iterator<T> operator++(int) {
    random_access_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(random_access_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(random_access_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return bIt::operator->(); }
  random_access_iterator<T> &operator--() {
    this->_ptr--;
    return *this;
  };
  random_access_iterator<T> operator--(int) {
    random_access_iterator<T> tmp(*this);
    --*this;
    return tmp;
  };
  random_access_iterator<T> &operator+=(difference_type sz) {
    while (sz--)
      ++*this;
    return *this;
  };
  random_access_iterator<T> operator+(difference_type sz) const {
    random_access_iterator<T> tmp(*this);
    tmp += sz;
    return tmp;
  };
  friend random_access_iterator<T>
  operator+(difference_type sz, random_access_iterator<T> const &src) {
    return (src + sz);
  };
  random_access_iterator<T> &operator-=(difference_type sz) {
    while (sz--)
      --*this;
    return *this;
  };
  random_access_iterator<T> operator-(difference_type sz) const {
    random_access_iterator<T> tmp(*this);
    tmp -= sz;
    return tmp;
  };
  difference_type operator-(random_access_iterator<T> src) const {
    difference_type n = 0;
    while (*this != src--)
      n++;
    return n;
  };
  bool operator<(random_access_iterator<T> const &rhs) {
    return rhs - *this > 0;
  };
  bool operator>(random_access_iterator<T> const &rhs) { return rhs < *this; };
  bool operator<=(random_access_iterator<T> const &rhs) {
    return !(*this < rhs);
  };
  bool operator>=(random_access_iterator<T> const &rhs) {
    return !(*this > rhs);
  };
  reference operator[](difference_type sz) const { return this->_ptr[sz]; };
};
template <class T>
class const_random_access_iterator
    : virtual public const_bidirectional_iterator<T>,
      virtual public const_iterator<random_access_iterator_tag, T> {

public:
  typedef iterator<random_access_iterator_tag, T> It;
  typedef bidirectional_iterator<T> bIt;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  const_random_access_iterator<T>() : bidirectional_iterator<T>(), It() {}

  const_random_access_iterator<T> &operator++() {
    It::operator++();
    return *this;
  }
  const_random_access_iterator<T> operator++(int) {
    const_random_access_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual bool operator==(const_random_access_iterator<T> const &rhs) {
    return (this->_ptr == rhs._ptr);
  }
  virtual bool operator!=(const_random_access_iterator<T> const &rhs) {
    return !(*this == rhs);
  }
  virtual value_type operator*() const { return It::operator*(); }
  virtual pointer operator->() const { return bIt::operator->(); }
  const_random_access_iterator<T> &operator--() {
    this->_ptr--;
    return *this;
  };
  const_random_access_iterator<T> operator--(int) {
    const_random_access_iterator<T> tmp(*this);
    --*this;
    return tmp;
  };
  const_random_access_iterator<T> &operator+=(difference_type sz) {
    while (sz--)
      ++*this;
    return *this;
  };
  const_random_access_iterator<T> operator+(difference_type sz) const {
    const_random_access_iterator<T> tmp(*this);
    tmp += sz;
    return tmp;
  };
  friend const_random_access_iterator<T>
  operator+(difference_type sz, const_random_access_iterator<T> const &src) {
    return (src + sz);
  };
  const_random_access_iterator<T> &operator-=(difference_type sz) {
    while (sz--)
      --*this;
    return *this;
  };
  const_random_access_iterator<T> operator-(difference_type sz) const {
    const_random_access_iterator<T> tmp(*this);
    tmp -= sz;
    return tmp;
  };
  difference_type operator-(const_random_access_iterator<T> src) const {
    difference_type n = 0;
    while (*this != src--)
      n++;
    return n;
  };
  bool operator<(const_random_access_iterator<T> const &rhs) {
    return rhs - *this > 0;
  };
  bool operator>(const_random_access_iterator<T> const &rhs) {
    return rhs < *this;
  };
  bool operator<=(const_random_access_iterator<T> const &rhs) {
    return !(*this < rhs);
  };
  bool operator>=(const_random_access_iterator<T> const &rhs) {
    return !(*this > rhs);
  };
  reference operator[](difference_type sz) const { return this->_ptr[sz]; };
};

//  Iterator Functions //
//---------------------------------------//
//  advance //
// For input
template <class Iter, class Distance>
typename ft::enable_if<Iter::iterator_category == input_iterator_tag(),
                       void>::type
advance(Iter &it, Distance n) {
  while (n--)
    it++;
}
// For bidir
template <class Iter, class Distance>
typename ft::enable_if<Iter::iterator_category == bidirectional_iterator_tag(),
                       void>::type
advance(Iter &it, Distance n) {
  if (n < 0)
    while (n++)
      it--;
  else
    while (n--)
      it++;
}
// For random access
template <class Iter, class Distance>
typename ft::enable_if<Iter::iterator_category == random_access_iterator_tag(),
                       void>::type
advance(Iter &it, Distance n) {
  if (n < 0)
    it -= -n;
  else
    it += n;
}

//  prev //
// For bidir
template <class Iter>
typename ft::enable_if<Iter::iterator_category == bidirectional_iterator_tag(),
                       Iter>::type
prev(Iter &it, typename iterator_traits<Iter>::difference_type n = 1) {
  advance(it, -n);
  return it;
}
// For random access
template <class Iter>
typename ft::enable_if<Iter::iterator_category == random_access_iterator_tag(),
                       Iter>::type
prev(Iter &it, typename iterator_traits<Iter>::difference_type n = 1) {
  advance(it, -n);
  return it;
}

//  distance //
// For any except random access
template <class Iter>
typename ft::enable_if<!(Iter::iterator_category ==
                         random_access_iterator_tag()),
                       typename iterator_traits<Iter>::difference_type>::type
distance(Iter first, Iter last) {
  typename iterator_traits<Iter>::difference_type dist = 0;
  while (first++ != last)
    dist++;
  return dist;
}
// For random access
template <class Iter>
typename ft::enable_if<Iter::iterator_category == random_access_iterator_tag(),
                       typename iterator_traits<Iter>::difference_type>::type
distance(Iter first, Iter last) {
  typename iterator_traits<Iter>::difference_type dist = first - last;
  return dist;
}

//  Reverse (Const) Iterator //
//---------------------------------------//
template <class Iterator> class reverse_iterator {

public:
  typedef Iterator iterator_type;
  typedef typename iterator_traits<Iterator>::value_type value_type;
  typedef typename iterator_traits<Iterator>::difference_type difference_type;
  typedef typename iterator_traits<Iterator>::pointer pointer;
  typedef typename iterator_traits<Iterator>::reference reference;
  typedef
      typename iterator_traits<Iterator>::iterator_category iterator_category;

  constexpr reverse_iterator() {}
  constexpr explicit reverse_iterator(iterator_type it) : baseIt(it) {}
  constexpr reverse_iterator(reverse_iterator<Iterator> const &revIt)
      : baseIt(revIt.baseIt) {}

  iterator_type base() const { return baseIt; }
  // arithmetic
  constexpr reverse_iterator &operator++() {
    --baseIt;
    return *this;
  }
  constexpr reverse_iterator operator++(int) {
    reverse_iterator<Iterator> tmp(baseIt);
    ++*this;
    return tmp;
  }
  constexpr reverse_iterator &operator--() {
    ++baseIt;
    return *this;
  }
  constexpr reverse_iterator operator--(int) {
    reverse_iterator<Iterator> tmp(baseIt);
    --*this;
    return tmp;
  }
  typename ft::enable_if<Iterator::iterator_category ==
                             random_access_iterator_tag(),
                         reverse_iterator &>::type
  operator+=(difference_type sz) {
    while (sz--)
      ++*this;
    return *this;
  }
  typename ft::enable_if<Iterator::iterator_category ==
                             random_access_iterator_tag(),
                         reverse_iterator>::type
  operator+(difference_type sz) {
    reverse_iterator<Iterator> tmp(*this);
    tmp += sz;
    return tmp;
  }
  typename ft::enable_if<Iterator::iterator_category ==
                             random_access_iterator_tag(),
                         reverse_iterator &>::type
  operator-=(difference_type sz) {
    while (sz--)
      --*this;
    return *this;
  }
  typename ft::enable_if<Iterator::iterator_category ==
                             random_access_iterator_tag(),
                         reverse_iterator>::type
  operator-(difference_type sz) {
    reverse_iterator<Iterator> tmp(*this);
    tmp -= sz;
    return tmp;
  }
  friend typename ft::enable_if<Iterator::iterator_category ==
                                    random_access_iterator_tag(),
                                reverse_iterator>::type
  operator+(difference_type sz, reverse_iterator const &src) {
    return (src + sz);
  }
  typename ft::enable_if<Iterator::iterator_category ==
                             random_access_iterator_tag(),
                         difference_type>::type
  operator-(reverse_iterator src) const {
    difference_type n = 0;
    while (*this != src--)
      n++;
    return n;
  }
  // booleans
  friend bool operator==(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() == b.base());
  };
  friend bool operator!=(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base != b.base());
  };
  friend typename ft::enable_if<
      Iterator::iterator_category == random_access_iterator_tag(), bool>::type
  operator<(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() < b.base());
  };
  friend typename ft::enable_if<
      Iterator::iterator_category == random_access_iterator_tag(), bool>::type
  operator>(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base > b.base);
  };
  friend typename ft::enable_if<
      Iterator::iterator_category == random_access_iterator_tag(), bool>::type
  operator<=(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() <= b.base());
  };
  friend typename ft::enable_if<
      Iterator::iterator_category == random_access_iterator_tag(), bool>::type
  operator>=(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() >= b.base());
  };
  // access
  constexpr reference operator*() const { return *prev(baseIt); }
  constexpr pointer operator->() const { return addressof(operator*()); }
  typename ft::enable_if<Iterator::iterator_category ==
                             random_access_iterator_tag(),
                         reference>::type
  operator[](difference_type n) const {
    return baseIt[-n - 1];
  }

protected:
  iterator_type baseIt;
};
} // namespace ft

#endif