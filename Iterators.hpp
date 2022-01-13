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
# include <iterator>

namespace ft {
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
        typedef std::random_access_iterator_tag     iterator_category;

    };

    //  const T* specialization
    template<class T>
    struct iterator_traits<const T*> {
        typedef std::ptrdiff_t                      difference_type;
        typedef T                                   value_type;
        typedef const T*                            pointer;
        typedef const T&                            reference;
        typedef std::random_access_iterator_tag     iterator_category;


    };

    //  Iterator //
    //---------------------------------------//
    template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
    struct iterator : public iterator_traits<T> {
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
        typedef Category  iterator_category;
        iterator(const iterator &) {};
        iterator&   operator=(const iterator & rhs) {
            if (this != &rhs)
                *this = rhs;
            return *this; 
        }
    };
}

#endif