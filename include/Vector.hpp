/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/02/24 15:53:41 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Iterators.hpp"
#include <cstddef>
#include <memory>

namespace ft {

template <class T, class Allocator = std::allocator<T>> class vector {
public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;

  class iterator : public ft::iterator<random_access_iterator_tag, T> {
  public:
    explicit iterator(pointer ptr) : _ptr(ptr){};
    iterator(iterator const &src) : _ptr(src._ptr){};
    ~iterator(){};
    iterator &operator=(const iterator &rhs) {
      if (*this != rhs)
        this->_ptr = rhs._ptr;
    };

    iterator &operator++() {
      _ptr++;
      return *this;
    }
    iterator operator++(int) {
      iterator tmp(*this);
      _ptr++;
      return tmp;
    }
    iterator &operator--() {
      this->_ptr--;
      return *this;
    };
    iterator operator--(int) {
      iterator tmp(*this);
      --*this;
      return tmp;
    };
    iterator &operator+=(difference_type sz) {
      while (sz--)
        ++*this;
      return *this;
    };
    iterator operator+(difference_type sz) const {
      iterator tmp(*this);
      tmp += sz;
      return tmp;
    };
    friend iterator operator+(difference_type sz, iterator const &src) {
      return (src + sz);
    };
    iterator &operator-=(difference_type sz) {
      while (sz--)
        --*this;
      return *this;
    };
    iterator operator-(difference_type sz) const {
      iterator tmp(*this);
      tmp -= sz;
      return tmp;
    };
    difference_type operator-(iterator src) const {
      difference_type n = 0;
      while (*this != src--)
        n++;
      return n;
    }
    bool operator==(iterator const &rhs) { return (this->_ptr == rhs._ptr); }
    bool operator!=(iterator const &rhs) { return !(*this == rhs); }
    bool operator<(iterator const &rhs) { return rhs - *this > 0; };
    bool operator>(iterator const &rhs) { return rhs < *this; };
    bool operator<=(iterator const &rhs) { return !(*this < rhs); };
    bool operator>=(iterator const &rhs) { return !(*this > rhs); };
    value_type operator*() const { return *_ptr; }
    pointer operator->() const { return _ptr; }
    reference operator[](difference_type sz) const { return this->_ptr[sz]; };

  protected:
    pointer _ptr;
  };
  class const_iterator
      : public ft::const_iterator<random_access_iterator_tag, T> {
  public:
    explicit const_iterator(pointer ptr) : _ptr(ptr){};
    const_iterator(const_iterator const &src) : _ptr(src._ptr){};
    ~const_iterator(){};
    const_iterator &operator=(const const_iterator &rhs) {
      if (*this != rhs)
        this->_ptr = rhs._ptr;
    };

    const_iterator &operator++() {
      _ptr++;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      _ptr++;
      return tmp;
    }
    const_iterator &operator--() {
      this->_ptr--;
      return *this;
    };
    const_iterator operator--(int) {
      const_iterator tmp(*this);
      --*this;
      return tmp;
    };
    const_iterator &operator+=(difference_type sz) {
      while (sz--)
        ++*this;
      return *this;
    };
    const_iterator operator+(difference_type sz) const {
      const_iterator tmp(*this);
      tmp += sz;
      return tmp;
    };
    friend const_iterator operator+(difference_type sz,
                                    const_iterator const &src) {
      return (src + sz);
    };
    const_iterator &operator-=(difference_type sz) {
      while (sz--)
        --*this;
      return *this;
    };
    const_iterator operator-(difference_type sz) const {
      const_iterator tmp(*this);
      tmp -= sz;
      return tmp;
    };
    difference_type operator-(const_iterator src) const {
      difference_type n = 0;
      while (*this != src--)
        n++;
      return n;
    }
    bool operator==(const_iterator const &rhs) {
      return (this->_ptr == rhs._ptr);
    }
    bool operator!=(const_iterator const &rhs) { return !(*this == rhs); }
    bool operator<(const_iterator const &rhs) { return rhs - *this > 0; };
    bool operator>(const_iterator const &rhs) { return rhs < *this; };
    bool operator<=(const_iterator const &rhs) { return !(*this < rhs); };
    bool operator>=(const_iterator const &rhs) { return !(*this > rhs); };
    value_type operator*() const { return *_ptr; }
    pointer operator->() const { return _ptr; }
    reference operator[](difference_type sz) const { return this->_ptr[sz]; };

  protected:
    pointer _ptr;
  };

  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  vector() : Allocator() {}
  explicit vector(const Allocator &alloc) {}
  explicit vector(size_type count, const T &value = T(),
                  const Allocator &alloc = Allocator()) {}
  template <class InputIt>
  vector(InputIt first, InputIt last, const Allocator &alloc = Allocator()) {}
  vector(const vector &other) {}
};

} // namespace ft

#endif