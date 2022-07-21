/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 16:44:54 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/21 15:12:23 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
# define MAP_HPP

#include "Iterators.hpp"
#include "utility.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <exception>
#include <iterator>
#include <list>
#include <memory>
#include <stdexcept>
#include <iostream>
namespace ft {

template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<ft::pair<const Key, T> > >
class map {
public:
  typedef Key key_type;
  typedef T mapped_type;
  typedef std::pair<const Key, T> value_type;
  typedef Compare key_compare;
  typedef Alloc allocator_type;

  
};
}

#endif