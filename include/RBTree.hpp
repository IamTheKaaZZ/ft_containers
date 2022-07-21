/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RBTree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 15:13:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/21 18:07:53 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "Iterators.hpp"
#include "utility.hpp"
#include <cstddef>
#include <sys/types.h>
#include <memory>


namespace ft {

enum RBTreeColor { Red = false, Black = true };

struct RBTreeNodeBase {

  typedef RBTreeNodeBase *BasePtr;
  typedef const RBTreeNodeBase *Const_BasePtr;

  RBTreeColor color;
  BasePtr parent;
  BasePtr left;
  BasePtr right;

  static BasePtr minimum(BasePtr x) {
    while (x->left != NULL) {
      x = x->left;
    }
    return x;
  }
  static Const_BasePtr minimum(Const_BasePtr x) {
    while (x->left != NULL) {
      x = x->left;
    }
    return x;
  }
  static BasePtr maximum(BasePtr x) {
    while (x->right != NULL) {
      x = x->right;
    }
    return x;
  }
  static Const_BasePtr maximum(Const_BasePtr x) {
    while (x->right != NULL) {
      x = x->right;
    }
    return x;
  }
};

template <typename KeyCompare> struct RBTreeKeyCompare {

  KeyCompare keyCompare;

  RBTreeKeyCompare() : keyCompare() {}
  RBTreeKeyCompare(const KeyCompare &comp) : keyCompare(comp) {}
  RBTreeKeyCompare(const RBTreeKeyCompare &other)
      : keyCompare(other.keyCompare) {}
};

struct RBTreeHeader {
  RBTreeNodeBase header;
  size_t nodeCount; // Keeps track of tree size

  RBTreeHeader() {
    header.color = Red;
    reset();
  }

  void reset() {
    header.parent = NULL;
    header.left = &header;
    header.right = &header;
    nodeCount = 0;
  }

  void moveData(RBTreeHeader& other) {
    header.color = other.header.color;
    header.parent = other.header.parent;
    header.left = other.header.left;
    header.right = other.header.right;
    header.parent->parent = &header;
    nodeCount = other.nodeCount;
  }
};

template<typename Value>
struct RBTreeNode : public RBTreeNodeBase {
    Value valueField;
    Value* valPtr() { return ft::addressof(valueField); }
    const Value* valPtr() const { return ft::addressof(valueField); }
};

template <typename Key, typename Value, typename KeyOfValue, typename Compare,
          typename Alloc = std::allocator<Value>>
class RBTree {
public:
  typedef Key key_type;
  typedef Value value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef Alloc allocator_type;
  typedef std::allocator<RBTreeNode<Value>> node_allocator_type;

  node_allocator_type &get_node_allocator_type() { return internalData.alloc; }
  const node_allocator_type &get_node_allocator_type() const {
    return internalData.alloc;
  }
  allocator_type get_allocator() const {
    return allocator_type(get_node_allocator_type());
  }
  typedef ft::bidirectional_iterator<value_type> iterator;
  typedef const ft::bidirectional_iterator<value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

protected:
  typedef RBTreeNodeBase *BasePtr;
  typedef const RBTreeNodeBase *Const_BasePtr;
  typedef RBTreeNode<Value> *Link_type;
  typedef const RBTreeNode<Value> *Const_Link_type;

  ///
  /// @brief Get the node object -> allocate a new node.
  ///
  /// @return Link_type
  ///
  Link_type get_node() { return get_node_allocator_type().allocate(1); }

  ///
  /// @brief Deallocates a node object.
  ///
  /// @param p
  ///
  void put_node(Link_type p) {
    get_node_allocator_type().deallocate(get_node_allocator_type(), p, 1);
  }

  ///
  /// @brief Construct a given node with a value.
  ///
  /// @param node
  /// @param x
  ///
  void construct_node(Link_type node, const value_type &x) {
    get_allocator().construct(node->valPtr(), x);
  }

  ///
  /// @brief Create a node object: allocate and construct it.
  ///
  /// @param x
  /// @return Link_type
  ///
  Link_type create_node(const value_type &x) {
    Link_type tmp = get_node();
    construct_node(tmp, x);
    return tmp;
  }

  ///
  /// @brief Destroy a node, does not free memory.
  ///
  /// @param p
  ///
  void destroy_node(Link_type p) { get_allocator().destroy(p->valPtr()); }

  ///
  /// @brief Destroy and deallocate a node.
  ///
  /// @param p
  ///
  void drop_node(Link_type p) {
    destroy_node(p);
    put_node(p);
  }

  ///
  /// @brief Clone a node object.
  ///
  /// @tparam NodeGen
  /// @param x
  /// @param node_gen
  /// @return Link_type
  ///
  template <typename NodeGen>
  Link_type clone_node(Const_Link_type x, NodeGen &node_gen) {
    Link_type tmp = node_gen(*x->valPtr());
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
  }

  ///
  /// @brief Internal data struct for the RBTree.
  ///
  /// @tparam KeyCompare
  ///
  template <typename KeyCompare>
  struct RBTreeInternal : public RBTreeKeyCompare<KeyCompare>,
                          public RBTreeHeader {
    node_allocator_type alloc;
    RBTreeInternal() : alloc() {}
    RBTreeInternal(const RBTreeInternal &other)
        : alloc(other.alloc), RBTreeKeyCompare<KeyCompare>(other.keyCompare) {}
    RBTreeInternal(const KeyCompare &comp, const allocator_type &a)
        : alloc(a), RBTreeKeyCompare<KeyCompare>(comp) {}
  };

  RBTreeInternal<Compare> internalData;

  BasePtr &root() { return internalData.header.parent; }
  Const_BasePtr root() const { return internalData.header.parent; }
  BasePtr &leftmost() { return internalData.header.left; }
  Const_BasePtr leftmost() const { return internalData.header.left; }
  BasePtr &rightmost() { return internalData.header.right; }
  Const_BasePtr rightmost() const { return internalData.header.right; }
  Link_type begin() {
    return static_cast<Link_type>(internalData.header.parent);
  }
  Const_Link_type begin() const {
    return static_cast<Const_Link_type>(internalData.header.parent);
  }
  BasePtr end() { return &internalData.header; }
  Const_BasePtr end() const { return &internalData.header; }
  static const_reference value(Const_Link_type x) { return *x->valptr(); }
  static const Key &key(Const_Link_type x) { return KeyOfValue()(value(x)); }
  static Link_type left(BasePtr x) { return static_cast<Link_type>(x->left); }
  static Const_Link_type left(Const_BasePtr x) {
    return static_cast<Const_Link_type>(x->left);
  }
  static Link_type right(BasePtr x) { return static_cast<Link_type>(x->right); }
  static Const_Link_type right(Const_BasePtr x) {
    return static_cast<Const_Link_type>(x->right);
  }
  static const_reference value(Const_BasePtr x) {
    return *static_cast<Const_Link_type>(x)->valptr();
  }
  static const Key &key(Const_BasePtr x) { return KeyOfValue()(value(x)); }
  static BasePtr minimum(BasePtr x) { return RBTreeNodeBase::minimum(x); }
  static Const_BasePtr minimum(Const_BasePtr x) {
    return RBTreeNodeBase::minimum(x);
  }
  static BasePtr maximum(BasePtr x) { return RBTreeNodeBase::maximum(x); }
  static Const_BasePtr maximum(Const_BasePtr x) {
    return RBTreeNodeBase::maximum(x);
  }

private: // Functors
  ///
  /// @brief Functor that recycles a pool of nodes and uses allocation once a
  /// pool is empty.
  ///
  ///
  struct ReuseOrAllocNode {
    ReuseOrAllocNode(RBTree &t)
        : rootNode(t.rootNode()), nodes(t.rightmostNode()), tree(t) {
      if (rootNode) {
        rootNode->parent = 0;
        if (nodes->left)
          nodes = nodes->left;
      } else
        nodes = 0;
    }

    ~ReuseOrAllocNode() { tree.erase(static_cast<Link_type>(rootNode)); }
    template <typename Arg> Link_type operator()(const Arg &arg) {
      Link_type node = static_cast<Link_type>(extract());
      if (node) {
        tree.destroy_node(node);
        tree.construct_node(node, arg);
        return node;
      }
      return tree.create_node(arg);
    }

  private:
    BasePtr extract() {
      if (!nodes)
        return nodes;
      BasePtr node = nodes;
      nodes = nodes->parent;
      if (nodes) {
        if (nodes->right == node) {
          nodes->right = 0;
          if (nodes->left) {
            nodes = nodes->left;
            while (nodes->right)
              nodes = nodes->right;
            if (nodes->left)
              nodes = nodes->left;
          }
        } else // node is on the left.
          nodes->left = 0;
      } else
        rootNode = 0;
      return node;
    }
    BasePtr rootNode;
    BasePtr nodes;
    RBTree &tree;
  };

  ///
  /// @brief Functor that doesn't recycle a pool of nodes.
  ///
  ///
  struct AllocNode {
    AllocNode(RBTree &t) : tree(t) {}
    template <typename Arg> Link_type operator()(const Arg &arg) const {
      return tree.create_node(arg);
    }

  private:
    RBTree &tree;
  };
};
}

#endif
