/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Iterators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 17:13:16 by bcosters          #+#    #+#             */
/*   Updated: 2022/06/23 17:31:06 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include "utility.hpp"
#include <cstddef>

namespace ft {

// @paragraph it_tags Iterator_tags //
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// @paragraph it_traits Iterator_traits //
template <class Iter> struct iterator_traits {
  typedef typename Iter::difference_type difference_type;
  typedef typename Iter::value_type value_type;
  typedef typename Iter::pointer pointer;
  typedef typename Iter::reference reference;
  typedef typename Iter::iterator_category iterator_category;
};

// @brief T *specialization
template <class T> struct iterator_traits<T *> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef random_access_iterator_tag iterator_category;
};

// @brief const T *specialization
template <class T> struct iterator_traits<const T *> {
  typedef std::ptrdiff_t difference_type;
  typedef T value_type;
  typedef const T *pointer;
  typedef const T &reference;
  typedef random_access_iterator_tag iterator_category;
};

// @paragraph base_it Base Iterator and Const Iterator //
/**
 * @brief Base (const) iterator class, comments are possible implementations
 *
 * @tparam Category = iterator_tag
 * @tparam T = data type
 * @tparam Distance = difference type, default ptrdiff_t
 * @tparam * = pointer, default is pointer to data type
 * @tparam & = reference, default is reference to data type
 */
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T *, class Reference = T &>
class iterator {

public:
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef Category iterator_category;

  //   explicit iterator(pointer ptr) : _ptr(ptr){};
  //   iterator(iterator const &src) : _ptr(src._ptr){};
  virtual ~iterator(){};
  //   iterator &operator=(const iterator &rhs) {
  //     if (*this != rhs)
  //       this->_ptr = rhs._ptr;
  //   };

  //   //  increment/decrement
  //   iterator &operator++() {
  //     _ptr++;
  //     return *this;
  //   }
  //   iterator operator++(int) {
  //     iterator tmp(*this);
  //     _ptr++;
  //     return tmp;
  //   }
  //   virtual iterator &operator--() = 0;
  //   virtual iterator operator--(int) = 0;
  //   arithmatic
  //   virtual iterator &operator+=(difference_type) = 0;
  //   virtual iterator operator+(difference_type) const = 0;
  //   virtual friend iterator operator+(difference_type, iterator const &) = 0;
  //   virtual iterator &operator-=(difference_type) = 0;
  //   virtual iterator operator-(difference_type) const = 0;
  //   virtual difference_type operator-(iterator) const = 0;
  //   boolean
  //   virtual friend bool operator==(iterator const &, iterator const &) = 0;
  //   virtual friend bool operator!=(iterator const &, iterator const &) = 0;
  //   virtual friend bool operator<(iterator const &, iterator const &) const =
  //   0; virtual friend bool operator>(iterator const &, iterator const &)
  //   const = 0; virtual friend bool operator<=(iterator const &, iterator
  //   const &) const = 0; virtual friend bool operator>=(iterator const &,
  //   iterator const &) const = 0; access
  //   reference operator*() const { return *(this->_ptr); }
  //   virtual pointer operator->() const = 0;
  //   virtual reference operator[](difference_type) const = 0;

  // protected:
  //   pointer _ptr;
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

  //   explicit const_iterator(pointer ptr) : _ptr(ptr){};
  //   const_iterator(const_iterator const &src) : _ptr(src._ptr){};
  //   const_iterator(iterator<Category, T> const &src) : _ptr(src._ptr){};
  virtual ~const_iterator(){};
  //   const_iterator &operator=(const const_iterator &rhs) {
  //     if (*this != rhs)
  //       this->_ptr = rhs._ptr;
  //   };

  //   //   increment / decrement
  //   virtual const_iterator &operator++() {
  //     _ptr++;
  //     return *this;
  //   }
  //   virtual const_iterator operator++(int) {
  //     const_iterator tmp(*this);
  //     _ptr++;
  //     return tmp;
  //   }
  //   virtual const_iterator &operator--() = 0;
  //   virtual const_iterator operator--(int) = 0;
  //   //   arithmatic
  //   virtual const_iterator &operator+=(difference_type) = 0;
  //   virtual const_iterator operator+(difference_type) const = 0;
  //   virtual friend const_iterator operator+(difference_type,
  //                                           const_iterator const &) = 0;
  //   virtual const_iterator &operator-=(difference_type) = 0;
  //   virtual const_iterator operator-(difference_type) const = 0;
  //   virtual difference_type operator-(const_iterator) const = 0;
  //   //   boolean
  //   virtual friend bool operator==(const_iterator const &,
  //                                  const_iterator const &) = 0;
  //   virtual friend bool operator!=(const_iterator const &,
  //                                  const_iterator const &) = 0;
  //   virtual friend bool operator<(const_iterator const &,
  //                                 const_iterator const &) const = 0;
  //   virtual friend bool operator>(const_iterator const &,
  //                                 const_iterator const &) const = 0;
  //   virtual friend bool operator<=(const_iterator const &,
  //                                  const_iterator const &) const = 0;
  //   virtual friend bool operator>=(const_iterator const &,
  //                                  const_iterator const &) const = 0;
  //   access
  //   virtual reference operator*() const { return *(this->_ptr); }
  //   virtual pointer operator->() const = 0;
  //   virtual reference operator[](difference_type) const = 0;

  // protected:
  //   pointer _ptr;
};

// @paragraph input_it Input(Const) Iterator

/**
 * @brief Commented example of input iterator and what it should implement
 *
 * @tparam T = data type
 */
template <class T>
class input_iterator : virtual public iterator<input_iterator_tag, T> {

public:
  typedef iterator<input_iterator_tag, T> It;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  explicit input_iterator(pointer p) : _ptr(p) {}
  input_iterator(const input_iterator &src) : _ptr(src._ptr) {}
  input_iterator &operator++() {
    this->_ptr++();
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
  virtual value_type operator*() const { return *this->_ptr; }
  virtual pointer operator->() const { return this->_ptr; }

protected:
  pointer _ptr;
};

/**
 * @brief Commented example of const input iterator and what it should implement
 *
 * @tparam T = data type
 */
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

  explicit const_input_iterator(pointer p) : _ptr(p) {}
  const_input_iterator(const const_input_iterator &src) : _ptr(src._ptr) {}
  const_input_iterator &operator++() {
    this->_ptr++();
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
  virtual value_type operator*() const { return *this->_ptr; }
  virtual pointer operator->() const { return this->_ptr; }

protected:
  pointer _ptr;
};

// @paragraph output_it Output(Const) Iterator

/**
 * @brief Commented example of output iterator and what it should implement
 *
 * @tparam T = data type
 */
template <class T>
class output_iterator : virtual public iterator<output_iterator_tag, T> {

public:
  typedef iterator<output_iterator_tag, T> It;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

  explicit output_iterator(pointer p) : _ptr(p) {}
  output_iterator(const output_iterator &src) : _ptr(src._ptr) {}
  output_iterator &operator++() {
    this->_ptr++();
    return *this;
  }
  output_iterator operator++(int) {
    output_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual reference operator*() const { return *(this->_ptr); }

protected:
  pointer _ptr;
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

  explicit const_output_iterator(pointer p) : _ptr(p) {}
  const_output_iterator(const const_output_iterator &src) : _ptr(src._ptr) {}
  const_output_iterator &operator++() {
    this->_ptr++();
    return *this;
  }
  const_output_iterator operator++(int) {
    const_output_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  virtual reference operator*() const { return *(this->_ptr); }

protected:
  pointer _ptr;
};

// @paragraph forward_it Forward(Const) Iterator

/**
 * @brief Commented example of (const) forward iterator and what it should
 * implement
 *
 * @tparam T = data type
 */
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
    this->_ptr(NULL);
  }

  forward_iterator &operator++() {
    inIt::operator++();
    return *this;
  }
  forward_iterator operator++(int) {
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
  virtual value_type operator*() const { return inIt::operator*(); }
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
      this->_ptr(NULL);
    }

    const_forward_iterator &operator++() {
      inIt::operator++();
      return *this;
    }
    const_forward_iterator operator++(int) {
      const_forward_iterator<T> tmp(*this);
      ++*this;
      return tmp;
    }
    virtual bool operator==(forward_iterator<T> const &rhs) {
      return (this->_ptr == rhs._ptr);
    }
    virtual bool operator!=(forward_iterator<T> const &rhs) {
      return !(*this == rhs);
    }
    virtual value_type operator*() const { return inIt::operator*(); }
    virtual pointer operator->() const { return inIt::operator->(); }
};

// @paragraph bidirect_it Bidirectional(Const) Iterator

/**
 * @brief Commented example of (const) bidirectional iterator and what it
 * should implement
 *
 * @tparam T = data type
 */
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
    fIt::operator++();
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
  virtual value_type operator*() const { return fIt::operator*(); }
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
    fIt::operator++();
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
  virtual value_type operator*() const { return fIt::operator*(); }
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

// @paragraph ra_it Random Access(Const) Iterator

/**
 * @brief Commented example of (const) random access iterator and what it
 * should implement
 *
 * @tparam T = data type
 */
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
    bIt::operator++();
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
  virtual value_type operator*() const { return bIt::operator*(); }
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
  typedef const_iterator<random_access_iterator_tag, T> It;
  typedef const_bidirectional_iterator<T> bIt;
  typedef typename It::value_type value_type;
  typedef typename It::distance difference_type;
  typedef typename It::pointer pointer;
  typedef typename It::reference reference;
  typedef typename It::iterator_category iterator_category;

    const_random_access_iterator<T>() : bidirectional_iterator<T>(), It()
    {}

    const_random_access_iterator<T> &operator++() {
      bIt::operator++();
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
    virtual value_type operator*() const { return bIt::operator*(); }
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
    operator+(difference_type sz, const_random_access_iterator<T> const
    &src) {
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
    reference operator[](difference_type sz) const { return this->_ptr[sz];
    };
};

// @paragraph it_fns Iterator Functions

/**
 * @brief iterator_category
 *
 * @tparam Iter
 * @return iterator_traits<Iter>::iterator_category
 */
template <class Iter>
typename iterator_traits<Iter>::iterator_category
__iterator_category(const Iter &) {
  return typename iterator_traits<Iter>::iterator_category();
}

/**
 * @brief _advance for input its
 *
 * @tparam Iter
 * @tparam Distance
 * @param it
 * @param n
 */
template <class Iter, class Distance>
void _advance(Iter &it, Distance n, input_iterator_tag) {
  while (n--)
    it++;
}

/**
 * @brief _advance for bidirect its
 *
 * @tparam Iter
 * @tparam Distance
 * @param it
 * @param n
 */
template <class Iter, class Distance>
void _advance(Iter &it, Distance n, bidirectional_iterator_tag) {
  if (n < 0)
    while (n++)
      it--;
  else
    while (n--)
      it++;
}

/**
 * @brief _advance for random access its
 *
 * @tparam Iter
 * @tparam Distance
 * @param it
 * @param n
 */
template <class Iter, class Distance>
void advance(Iter &it, Distance n, random_access_iterator_tag) {
  if (n < 0)
    it -= -n;
  else
    it += n;
}

/**
 * @brief Main advance function
 *
 * @tparam Iter
 * @tparam Distance
 * @param it
 * @param n
 */
template <class Iter, class Distance> void advance(Iter &it, Distance n) {
  typename iterator_traits<Iter>::difference_type d = n;
  _advance(it, d, __iterator_category(it));
}

/**
 * @brief _prev for bidirect_its
 *
 * @tparam Iter
 * @param it
 * @param n
 * @return Iter
 */
template <class Iter>
Iter _prev(bidirectional_iterator_tag, Iter &it,
           typename iterator_traits<Iter>::difference_type n = 1) {
  advance(it, -n);
  return it;
}

/**
 * @brief _prev for random_access_its
 *
 * @tparam Iter
 * @param it
 * @param n
 * @return Iter
 */
template <class Iter>
Iter _prev(random_access_iterator_tag, Iter &it,
           typename iterator_traits<Iter>::difference_type n = 1) {
  advance(it, -n);
  return it;
}

/**
 * @brief Main prev function
 *
 * @tparam Iter
 * @param it
 * @param n
 * @return Iter
 */
template <class Iter>
Iter prev(Iter &it, typename iterator_traits<Iter>::difference_type n = 1) {
  return _prev(ft::__iterator_category(it), it, n);
}

//  distance //
// For any except random access

/**
 * @brief _distance for any it except random access
 *
 * @tparam Iter
 * @param first
 * @param last
 * @return iterator_traits<Iter>::difference_type
 */
template <class Iter>
typename iterator_traits<Iter>::difference_type _distance(Iter first, Iter last,
                                                          input_iterator_tag) {
  typename iterator_traits<Iter>::difference_type dist = 0;
  while (first++ != last)
    dist++;
  return dist;
}

/**
 * @brief _distance specifically for random access its
 *
 * @tparam Iter
 * @param first
 * @param last
 * @return iterator_traits<Iter>::difference_type
 */
template <class Iter>
typename iterator_traits<Iter>::difference_type
_distance(Iter first, Iter last, random_access_iterator_tag) {
  typename iterator_traits<Iter>::difference_type dist = last - first;
  return dist;
}

/**
 * @brief Main distance function
 *
 * @tparam Iter
 * @param first
 * @param last
 * @return iterator_traits<Iter>::difference_type
 */
template <class Iter>
typename iterator_traits<Iter>::difference_type distance(Iter first,
                                                         Iter last) {
  return _distance(first, last, __iterator_category(first));
}

// Reverse(Const) Iterator
template <class Iterator>
class reverse_iterator
    : iterator<typename iterator_traits<Iterator>::iterator_category,
               typename iterator_traits<Iterator>::value_type,
               typename iterator_traits<Iterator>::difference_type,
               typename iterator_traits<Iterator>::pointer,
               typename iterator_traits<Iterator>::reference> {

public:
  typedef Iterator iterator_type;
  typedef typename iterator_traits<Iterator>::value_type value_type;
  typedef typename iterator_traits<Iterator>::difference_type difference_type;
  typedef typename iterator_traits<Iterator>::pointer pointer;
  typedef typename iterator_traits<Iterator>::reference reference;
  typedef
      typename iterator_traits<Iterator>::iterator_category iterator_category;

  reverse_iterator() : baseIt() {}
  explicit reverse_iterator(iterator_type it) : baseIt(it) {}
  reverse_iterator(reverse_iterator const &revIt) : baseIt(revIt.baseIt) {}
  template <class Iter>
  reverse_iterator(reverse_iterator<Iter> const &revIt)
      : baseIt(revIt.base()) {}

  iterator_type base() const { return baseIt; }
  // arithmetic
  reverse_iterator &operator++() {
    --baseIt;
    return *this;
  }
  reverse_iterator operator++(int) {
    reverse_iterator<Iterator> tmp(baseIt);
    ++*this;
    return tmp;
  }
  reverse_iterator &operator--() {
    ++baseIt;
    return *this;
  }
  reverse_iterator operator--(int) {
    reverse_iterator<Iterator> tmp(baseIt);
    --*this;
    return tmp;
  }
  reverse_iterator &operator+=(difference_type sz) {
    while (sz--)
      ++*this;
    return *this;
  }
  reverse_iterator operator+(difference_type sz) {
    reverse_iterator<Iterator> tmp(*this);
    tmp += sz;
    return tmp;
  }
  reverse_iterator &operator-=(difference_type sz) {
    while (sz--)
      --*this;
    return *this;
  }

  reverse_iterator operator-(difference_type sz) {
    reverse_iterator<Iterator> tmp(*this);
    tmp -= sz;
    return tmp;
  }
  friend reverse_iterator operator+(difference_type sz,
                                    reverse_iterator const &src) {
    return (src + sz);
  }

  difference_type operator-(reverse_iterator src) const {
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
    return (a.base() != b.base());
  };
  friend bool operator<(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() < b.base());
  };
  friend bool operator>(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() > b.base());
  };
  friend bool operator<=(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() <= b.base());
  };
  friend bool operator>=(reverse_iterator const &a, reverse_iterator const &b) {
    return (a.base() >= b.base());
  };
  // access
  reference operator*() const {
    iterator_type tmp = baseIt;
    return *--tmp;
  }
  pointer operator->() const { return addressof(operator*()); }
  reference operator[](difference_type n) const { return baseIt[-n - 1]; }

protected:
  iterator_type baseIt;
};
} // namespace ft

#endif