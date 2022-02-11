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
            iterator&   operator=(const iterator & rhs) = default;

            //increment/decrement
            virtual iterator&   operator++() { _ptr++; return *this; }
            virtual iterator    operator++(int) { iterator tmp(*this); _ptr++; return tmp; }
            virtual iterator&   operator--() = 0;
            virtual iterator    operator--(int) = 0;
            //arithmatic
            typedef size_t  size_type;
            virtual iterator&		operator+=(size_type) = 0;
            virtual iterator		operator+(size_type) const = 0;
            virtual friend iterator	operator+(size_type, iterator const &) = 0;
            virtual iterator&		operator-=(size_type) = 0;
            virtual iterator		operator-(size_type) const = 0;
            virtual difference_type	operator-(iterator) const = 0;
            //boolean
            virtual friend bool    operator==(iterator const &, iterator const &) = 0;
            virtual friend bool    operator!=(iterator const &, iterator const &) = 0;
            virtual friend bool    operator<(iterator const &, iterator const &) const = 0;
            virtual friend bool    operator>(iterator const &, iterator const &) const = 0;
            virtual friend bool    operator<=(iterator const &, iterator const &) const = 0;
            virtual friend bool    operator>=(iterator const &, iterator const &) const = 0;
            //access
            virtual reference   operator*() const { return *(this->_ptr); }
            virtual pointer     operator->() const = 0;
            virtual reference   operator[](size_type) const = 0;
        
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
            const_iterator&   operator=(const const_iterator & rhs) = default;

            //increment/decrement
            virtual const_iterator&   operator++() { _ptr++; return *this; }
            virtual const_iterator    operator++(int) { const_iterator tmp(*this); _ptr++; return tmp; }
            virtual const_iterator&   operator--() = 0;
            virtual const_iterator    operator--(int) = 0;
            //arithmatic
            typedef size_t  size_type;
            virtual const_iterator&			operator+=(size_type) = 0;
            virtual const_iterator			operator+(size_type) const = 0;
            virtual friend const_iterator	operator+(size_type, const_iterator const &) = 0;
            virtual const_iterator&			operator-=(size_type) = 0;
            virtual const_iterator			operator-(size_type) const = 0;
            virtual difference_type			operator-(const_iterator) const = 0;
            //boolean
            virtual friend bool    operator==(const_iterator const &, const_iterator const &) = 0;
            virtual friend bool    operator!=(const_iterator const &, const_iterator const &) = 0;
            virtual friend bool    operator<(const_iterator const &, const_iterator const &) const = 0;
            virtual friend bool    operator>(const_iterator const &, const_iterator const &) const = 0;
            virtual friend bool    operator<=(const_iterator const &, const_iterator const &) const = 0;
            virtual friend bool    operator>=(const_iterator const &, const_iterator const &) const = 0;
            //access
            virtual reference   operator*() const { return *(this->_ptr); }
            virtual pointer     operator->() const = 0;
            virtual reference   operator[](size_type) const = 0;
        
        protected:

            pointer _ptr;
    };

    //  Input (Const) Iterator //
    //---------------------------------------//
    template<class T>
    class input_iterator : virtual public iterator<input_iterator_tag, T> {

        public:

            virtual friend bool operator==(iterator const & lhs, iterator const & rhs) { return (lhs._ptr == rhs._ptr); }
            virtual friend bool operator!=(iterator const & lhs, iterator const & rhs) { return (lhs._ptr != rhs._ptr); }
            virtual value_type  operator*() const { return *(this->_ptr); }
            virtual pointer     operator->() const { return this->_ptr; }
    };

    template<class T>
    class const_input_iterator : virtual public const_iterator<input_iterator_tag, T> {

        public:

            virtual friend bool operator==(const_iterator const & lhs, const_iterator const & rhs) { return (lhs._ptr == rhs._ptr); }
            virtual friend bool operator!=(const_iterator const & lhs, const_iterator const & rhs) { return (lhs._ptr != rhs._ptr); }
            virtual value_type  operator*() const { return *(this->_ptr); }
            virtual pointer     operator->() const { return this->_ptr; }
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
            virtual reference   operator*() const { return *(this->_ptr); }
    };

    //  Forward (Const) Iterator //
    //---------------------------------------//
    template<class T>
    class forward_iterator :
        virtual public input_iterator<T>,
        virtual public output_iterator<T>,
        virtual public iterator<forward_iterator_tag, T> {

        public:
            forward_iterator() {
                this->_ptr(nullptr);
            }
    };
    template<class T>
    class const_forward_iterator :
        virtual public const_input_iterator<T>,
        virtual public const_iterator<forward_iterator_tag, T> {

        public:
            forward_iterator() {
                this->_ptr(nullptr);
            }
    };

    //  Bidirectional (Const) Iterator //
    //---------------------------------------//
    template<class T>
    class bidirectional_iterator :
        virtual public forward_iterator<T>,
        virtual public iterator<bidirectional_iterator_tag, T> {

        public:
            typedef iterator<bidirectional_iterator_tag, T> bidirect_it;
            virtual iterator&   operator--() {
                this->_ptr--;
                return *this;
            };
            virtual iterator    operator--(int) {
                bidirect_it tmp(*this);
                *this--;
                return tmp;
            };
    };
    template<class T>
    class const_bidirectional_iterator :
        virtual public const_forward_iterator<T>,
        virtual public const_iterator<bidirectional_iterator_tag, T> {

        public:
            typedef const_iterator<bidirectional_iterator_tag, T> bidirect_it;
            virtual const_iterator&   operator--() {
                this->_ptr--;
                return *this;
            };
            virtual const_iterator    operator--(int) {
                bidirect_it tmp(*this);
                *this--;
                return tmp;
            };
    };

    //  Random Access (Const) Iterator //
    //---------------------------------------//
    template<class T>
    class random_access_iterator :
        virtual public bidirectional_iterator<T>,
        virtual public iterator<random_access_iterator_tag, T> {

        public:
			typedef iterator<random_access_iterator_tag, T>	ra_it;
            iterator&		operator+=(size_type sz) {
				this->_ptr += sz;
				return *this;
			};
            iterator		operator+(size_type sz) const {
				ra_it	tmp(*this);
				tmp += sz;
				return tmp;
			};
            friend iterator operator+(size_type sz, iterator const & src) {
				return (src + sz);
			};
            iterator&		operator-=(size_type sz) {
				this->_ptr -= sz;
				return *this;
			};
            iterator		operator-(size_type sz) const {
				ra_it	tmp(*this);
				tmp -= sz;
				return tmp;
			};
            difference_type operator-(iterator src) const {
				return this->_ptr - src._ptr;
			};
            reference		operator[](size_type sz) const {
				return this->_ptr[sz];
			};
    };
    template<class T>
    class const_random_access_iterator :
        virtual public const_bidirectional_iterator<T>,
        virtual public const_iterator<random_access_iterator_tag, T> {

        public:
			typedef const_iterator<random_access_iterator_tag, T>	ra_it;
            const_iterator&		operator+=(size_type sz) {
				this->_ptr += sz;
				return *this;
			};
            const_iterator		operator+(size_type sz) const {
				ra_it	tmp(*this);
				tmp += sz;
				return tmp;
			};
            friend const_iterator operator+(size_type sz, const_iterator const & src) {
				return (src + sz);
			};
            const_iterator&		operator-=(size_type sz) {
				this->_ptr -= sz;
				return *this;
			};
            const_iterator		operator-(size_type sz) const {
				ra_it	tmp(*this);
				tmp -= sz;
				return tmp;
			};
            difference_type operator-(const_iterator src) const {
				return this->_ptr - src._ptr;
			};
            reference		operator[](size_type sz) const {
				return this->_ptr[sz];
			};
    };

    //  Reverse (Const) Iterator //
    //---------------------------------------//
	template<class Iterator>
	class reverse_iterator {

        public:

			typedef Iterator												iterator_type;
            typedef typename iterator_traits<Iterator>::value_type			value_type;
            typedef typename iterator_traits<Iterator>::difference_type		difference_type;
            typedef typename iterator_traits<Iterator>::pointer				pointer;
            typedef typename iterator_traits<Iterator>::reference			reference;
            typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;


	};
}

#endif