/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AllocatorTraits.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:37:18 by bcosters          #+#    #+#             */
/*   Updated: 2022/05/06 10:30:33 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <memory>

namespace ft {

/**
 * @brief  Uniform interface to C++98 and C++0x allocators.
 * @ingroup allocators
 */
template <typename Alloc> struct alloc_traits {
  typedef Alloc allocator_type;
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::const_pointer const_pointer;
  typedef typename Alloc::value_type value_type;
  typedef typename Alloc::reference reference;
  typedef typename Alloc::const_reference const_reference;
  typedef typename Alloc::size_type size_type;
  typedef typename Alloc::difference_type difference_type;
  static pointer allocate(Alloc &a, size_type n) { return a.allocate(n); }
  static void deallocate(Alloc &a, pointer p, size_type n) {
    a.deallocate(p, n);
  }
  template <typename _Tp>
  static void construct(Alloc &a, pointer p, const _Tp &arg) {
    a.construct(p, arg);
  }
  static void destroy(Alloc &a, pointer p) { a.destroy(p); }
  static size_type max_size(const Alloc &a) { return a.max_size(); }
  static const Alloc &select_on_copy(const Alloc &a) { return a; }


template<typename _Alloc>
    inline void __do_alloc_on_swap(_Alloc& __one, _Alloc& __two, true_type)
    {
      using std::swap;
      swap(__one, __two);
    }
  template<typename _Alloc>
    inline void __do_alloc_on_swap(_Alloc&, _Alloc&, false_type)
    { }
  template<typename _Alloc>
    inline void __alloc_on_swap(_Alloc& __one, _Alloc& __two)
    {
      typedef allocator_traits<_Alloc> __traits;
      typedef typename __traits::propagate_on_container_swap __pocs;
      __do_alloc_on_swap(__one, __two, __pocs());
    }


  static void on_swap(Alloc &a, Alloc &b) {
    std::__swap_allocator<Alloc>(a, b);
  }
  template <typename T> struct rebind {
    typedef typename Alloc::template rebind<T>::other other;
  };
};
} // namespace ft