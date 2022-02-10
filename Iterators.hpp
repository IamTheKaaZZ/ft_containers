/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Iterators.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 17:13:16 by bcosters          #+#    #+#             */
/*   Updated: 2022/01/13 18:02:46 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATORS_HPP
# define ITERATORS_HPP

# include <cstddef>
// # include <iterator>

namespace ft {

    //  Iterator_tags //
    //---------------------------------------//
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    //  Iterator_traits //
    //---------------------------------------//
    template<class Iter>
    struct iterator_traits {
        typedef typename Iter::difference_type      difference_type;
        typedef typename Iter::value_type           value_type;
        typedef typename Iter::pointer              pointer;
        typedef typename Iter::reference            reference;
        typedef typename Iter::iterator_category    iterator_category;

    };

    //  T* specialization
    template<class T>
    struct iterator_traits<T*> {
        typedef std::ptrdiff_t                      difference_type;
        typedef T                                   value_type;
        typedef T*                                  pointer;
        typedef T&                                  reference;
        typedef random_access_iterator_tag          iterator_category;

    };

    //  const T* specialization
    template<class T>
    struct iterator_traits<const T*> {
        typedef std::ptrdiff_t                      difference_type;
        typedef T                                   value_type;
        typedef const T*                            pointer;
        typedef const T&                            reference;
        typedef random_access_iterator_tag          iterator_category;


    };

    //  Base Iterator and Const Iterator //
    //---------------------------------------//
    template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
    class iterator {

        public:

            typedef T         value_type;
            typedef Distance  difference_type;
            typedef Pointer   pointer;
            typedef Reference reference;
            typedef Category  iterator_category;

            iterator() = delete;
            explicit iterator(pointer ptr) : _ptr(ptr) {};
            iterator(iterator const &) = default;
            virtual ~iterator() = default;
            iterator&   operator=(const iterator & rhs) = delete;

            //increment/decrement
            virtual iterator&   operator++() { _ptr++; return *this; }
            virtual iterator    operator++(int) { iterator tmp(*this); _ptr++; return tmp; }
            virtual iterator&   operator--() = 0;
            virtual iterator    operator--(int) = 0;
            //boolean
            virtual bool    operator==(iterator const &) = 0;
            virtual bool    operator!=(iterator const &) = 0;
            virtual bool    operator<(const iterator&) const = 0;
            virtual bool    operator>(const iterator&) const = 0;
            virtual bool    operator<=(const iterator&) const = 0;
            virtual bool    operator>=(const iterator&) const = 0;
            //access
            virtual reference   operator*() const { return *(this->_ptr); }
            virtual pointer     operator->() const = 0;
        
        protected:

            pointer _ptr;
    };

    template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
    class const_iterator {

        public:

            typedef T               value_type;
            typedef Distance        difference_type;
            typedef Pointer const   pointer;
            typedef Reference const reference;
            typedef Category        iterator_category;

            const_iterator() = delete;
            explicit const_iterator(pointer ptr) : _ptr(ptr) {};
            const_iterator(const_iterator const &) = default;
            const_iterator(iterator const & src) : _ptr(src._ptr) {};
            virtual ~const_iterator() = default;
            const_iterator&   operator=(const const_iterator & rhs) = delete;

            //increment/decrement
            virtual const_iterator&   operator++() { _ptr++; return *this; }
            virtual const_iterator    operator++(int) { const_iterator tmp(*this); _ptr++; return tmp; }
            virtual const_iterator&   operator--() = 0;
            virtual const_iterator    operator--(int) = 0;
            //boolean
            virtual bool    operator==(const_iterator const &) = 0;
            virtual bool    operator!=(const_iterator const &) = 0;
            virtual bool    operator<(const const_iterator&) const = 0;
            virtual bool    operator>(const const_iterator&) const = 0;
            virtual bool    operator<=(const const_iterator&) const = 0;
            virtual bool    operator>=(const const_iterator&) const = 0;
            //access
            virtual reference   operator*() const { return *(this->_ptr); }
            virtual pointer     operator->() const = 0;
        
        protected:

            pointer _ptr;
    };

    //  Input (Const) Iterator //
    //---------------------------------------//
    template<class T>
    class input_iterator : virtual public iterator<input_iterator_tag, T> {

        public:

            virtual bool        operator==(input_iterator const & rhs) { return (this->_ptr == rhs._ptr); }
            virtual bool        operator!=(input_iterator const & rhs) { return (this->_ptr != rhs._ptr); }
            virtual value_type   operator*() const { return *(this->_ptr); }
            virtual pointer     operator->() const { return this->_ptr; }
    };

    template<class T>
    class const_input_iterator : virtual public const_iterator<input_iterator_tag, T> {

        public:

            virtual bool            operator==(const_input_iterator const & rhs) { return (this->_ptr == rhs._ptr); }
            virtual bool            operator!=(const_input_iterator const & rhs) { return (this->_ptr != rhs._ptr); }
            virtual value_type      operator*() const { return *(this->_ptr); }
            virtual pointer         operator->() const { return this->_ptr; }
    };

    //  Output (Const) Iterator //
    //---------------------------------------//
    template<class T>
    class output_iterator : virtual public iterator<output_iterator_tag, T> {
        public:
            virtual reference   operator*() const { return *(this->_ptr); }
    };
    template<class T>
    class const_output_iterator : virtual public const_iterator<output_iterator_tag, T> {
        public:
            virtual reference const   operator*() const { return *(this->_ptr); }
    };

    //  Forward (Const) Iterator //
    //---------------------------------------//
}

#endif