/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RBTree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 15:13:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/25 16:01:17 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "Iterators.hpp"
#include "utility.hpp"
#include <cstddef>
#include <memory>
#include <sys/types.h>

///
/// @brief A red-black tree is a binary search tree with one extra attribute for
/// each node: the colour, which is either red or black.
///
///    It has following properties:
///    1. Every node is either red or black.
///    2. Every leaf (NULL) is black.
///    3. If a node is red, then both its children are black.
///    4. Every simple path from a node to a descendant leaf contains the same
///    number of black nodes.
///

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

///
/// @brief Helper struct for the comparison of keys.
///
/// @tparam KeyCompare
///
template <typename KeyCompare> struct RBTreeKeyCompare {

  KeyCompare keyCompare;

  RBTreeKeyCompare() : keyCompare() {}
  RBTreeKeyCompare(const KeyCompare &comp) : keyCompare(comp) {}
  RBTreeKeyCompare(const RBTreeKeyCompare &other)
      : keyCompare(other.keyCompare) {}
};

///
/// @brief Helper struct for default initialization.
///
///
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

  void moveData(RBTreeHeader &other) {
    header.color = other.header.color;
    header.parent = other.header.parent;
    header.left = other.header.left;
    header.right = other.header.right;
    header.parent->parent = &header;
    nodeCount = other.nodeCount;
  }
};

///
/// @brief Node struct.
///
/// @tparam Value
///
template <typename Value> struct RBTreeNode : public RBTreeNodeBase {
  Value valueField;
  Value *valPtr() { return ft::addressof(valueField); }
  const Value *valPtr() const { return ft::addressof(valueField); }
};

struct RBTree_operation_helper {
  static RBTreeNodeBase *increment_helper(RBTreeNodeBase *x) throw() {
    if (x->right != 0) {
      x = x->right;
      while (x->left != 0)
        x = x->left;
    } else {
      RBTreeNodeBase *y = x->parent;
      while (x == y->right) {
        x = y;
        y = y->parent;
      }
      if (x->right != y)
        x = y;
    }
    return x;
  }
  RBTreeNodeBase *increment(RBTreeNodeBase *x) throw() {
    return increment_helper(x);
  }
  const RBTreeNodeBase *increment(const RBTreeNodeBase *x) throw() {
    return increment_helper(const_cast<RBTreeNodeBase *>(x));
  }
  static RBTreeNodeBase *decrement_helper(RBTreeNodeBase *x) throw() {
    if (x->color == Red && x->parent->parent == x)
      x = x->right;
    else if (x->left != 0) {
      RBTreeNodeBase *y = x->left;
      while (y->right != 0)
        y = y->right;
      x = y;
    } else {
      RBTreeNodeBase *y = x->parent;
      while (x == y->left) {
        x = y;
        y = y->parent;
      }
      x = y;
    }
    return x;
  }
  RBTreeNodeBase *decrement(RBTreeNodeBase *x) throw() {
    return decrement_helper(x);
  }
  const RBTreeNodeBase *decrement(const RBTreeNodeBase *x) throw() {
    return decrement_helper(const_cast<RBTreeNodeBase *>(x));
  }
  static void rotate_left_helper(RBTreeNodeBase *const x,
                                 RBTreeNodeBase *&root) {
    RBTreeNodeBase *const y = x->right;
    x->right = y->left;
    if (y->left != 0)
      y->left->parent = x;
    y->parent = x->parent;
    if (x == root)
      root = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void rotate_left(RBTreeNodeBase *const x, RBTreeNodeBase *&root) {
    rotate_left_helper(x, root);
  }
  static void lrotate_right_helper(RBTreeNodeBase *const x,
                                   RBTreeNodeBase *&root) {
    RBTreeNodeBase *const y = x->left;
    x->left = y->right;
    if (y->right != 0)
      y->right->parent = x;
    y->parent = x->parent;
    if (x == root)
      root = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void rotate_right_helper(RBTreeNodeBase *const x, RBTreeNodeBase *&root) {
    rotate_right_helper(x, root);
  }

  void insert_and_rebalance(const bool insert_left, RBTreeNodeBase *x,
                            RBTreeNodeBase *p, RBTreeNodeBase &header) throw() {
    RBTreeNodeBase *&root = header.parent;
    // Initialize fields in new node to insert.
    x->parent = p;
    x->left = 0;
    x->right = 0;
    x->color = Red;
    // Insert.
    // Make new node child of parent and maintain root, leftmost and
    // rightmost nodes.
    // N.B. First node is always inserted left.
    if (insert_left) {
      p->left = x; // also makes leftmost = x when p == &header
      if (p == &header) {
        header.parent = x;
        header.right = x;
      } else if (p == header.left)
        header.left = x; // maintain leftmost pointing to min node
    } else {
      p->right = x;
      if (p == header.right)
        header.right = x; // maintain rightmost pointing to max node
    }
    // Rebalance.
    while (x != root && x->parent->color == Red) {
      RBTreeNodeBase *const xpp = x->parent->parent;
      if (x->parent == xpp->left) {
        RBTreeNodeBase *const y = xpp->right;
        if (y && y->color == Red) {
          x->parent->color = Black;
          y->color = Black;
          xpp->color = Red;
          x = xpp;
        } else {
          if (x == x->parent->right) {
            x = x->parent;
            rotate_left_helper(x, root);
          }
          x->parent->color = Black;
          xpp->color = Red;
          rotate_right_helper(xpp, root);
        }
      } else {
        RBTreeNodeBase *const y = xpp->left;
        if (y && y->color == Red) {
          x->parent->color = Black;
          y->color = Black;
          xpp->color = Red;
          x = xpp;
        } else {
          if (x == x->parent->left) {
            x = x->parent;
            rotate_right_helper(x, root);
          }
          x->parent->color = Black;
          xpp->color = Red;
          rotate_left_helper(xpp, root);
        }
      }
    }
    root->color = Black;
  }
  RBTreeNodeBase *rebalance_for_erase(RBTreeNodeBase *const z,
                                      RBTreeNodeBase &header) throw() {
    RBTreeNodeBase *&root = header.parent;
    RBTreeNodeBase *&leftmost = header.left;
    RBTreeNodeBase *&rightmost = header.right;
    RBTreeNodeBase *y = z;
    RBTreeNodeBase *x = 0;
    RBTreeNodeBase *x_parent = 0;
    if (y->left == 0)       // z has at most one non-null child. y == z.
      x = y->right;         // x might be null.
    else if (y->right == 0) // z has exactly one non-null child. y == z.
      x = y->left;          // x is not null.
    else {
      // z has two non-null children.  Set y to
      y = y->right; //   z's successor.  x might be null.
      while (y->left != 0)
        y = y->left;
      x = y->right;
    }
    if (y != z) {
      // relink y in place of z.  y is z's successor
      z->left->parent = y;
      y->left = z->left;
      if (y != z->right) {
        x_parent = y->parent;
        if (x)
          x->parent = y->parent;
        y->parent->left = x; // y must be a child of left
        y->right = z->right;
        z->right->parent = y;
      } else
        x_parent = y;
      if (root == z)
        root = y;
      else if (z->parent->left == z)
        z->parent->left = y;
      else
        z->parent->right = y;
      y->parent = z->parent;
      std::swap(y->color, z->color);
      y = z;
      // y now points to node to be actually deleted
    } else { // y == z
      x_parent = y->parent;
      if (x)
        x->parent = y->parent;
      if (root == z)
        root = x;
      else if (z->parent->left == z)
        z->parent->left = x;
      else
        z->parent->right = x;
      if (leftmost == z) {
        if (z->right == 0) // z->left must be null also
          leftmost = z->parent;
        // makes leftmost == header if z == root
        else
          leftmost = RBTreeNodeBase::minimum(x);
      }
      if (rightmost == z) {
        if (z->left == 0) // z->right must be null also
          rightmost = z->parent;
        // makes rightmost == header if z == root
        else // x == z->left
          rightmost = RBTreeNodeBase::maximum(x);
      }
    }
    if (y->color != Red) {
      while (x != root && (x == 0 || x->color == Black))
        if (x == x_parent->left) {
          RBTreeNodeBase *w = x_parent->right;
          if (w->color == Red) {
            w->color = Black;
            x_parent->color = Red;
            rotate_left_helper(x_parent, root);
            w = x_parent->right;
          }
          if ((w->left == 0 || w->left->color == Black) &&
              (w->right == 0 || w->right->color == Black)) {
            w->color = Red;
            x = x_parent;
            x_parent = x_parent->parent;
          } else {
            if (w->right == 0 || w->right->color == Black) {
              w->left->color = Black;
              w->color = Red;
              rotate_right_helper(w, root);
              w = x_parent->right;
            }
            w->color = x_parent->color;
            x_parent->color = Black;
            if (w->right)
              w->right->color = Black;
            rotate_left_helper(x_parent, root);
            break;
          }
        } else {
          // same as above, with right <-> left.
          RBTreeNodeBase *w = x_parent->left;
          if (w->color == Red) {
            w->color = Black;
            x_parent->color = Red;
            rotate_right_helper(x_parent, root);
            w = x_parent->left;
          }
          if ((w->right == 0 || w->right->color == Black) &&
              (w->left == 0 || w->left->color == Black)) {
            w->color = Red;
            x = x_parent;
            x_parent = x_parent->parent;
          } else {
            if (w->left == 0 || w->left->color == Black) {
              w->right->color = Black;
              w->color = Red;
              rotate_left_helper(w, root);
              w = x_parent->left;
            }
            w->color = x_parent->color;
            x_parent->color = Black;
            if (w->left)
              w->left->color = Black;
            rotate_right_helper(x_parent, root);
            break;
          }
        }
      if (x)
        x->color = Black;
    }
    return y;
  }
  unsigned int black_count(const RBTreeNodeBase *node,
                           const RBTreeNodeBase *root) throw() {
    if (node == 0)
      return 0;
    unsigned int sum = 0;
    do {
      if (node->color == Black)
        ++sum;
      if (node == root)
        break;
      node = node->parent;
    } while (1);
    return sum;
  }
};

template <typename T> struct RBTree_iterator {
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef RBTree_iterator<T> RBT_it;
  typedef RBTreeNodeBase::BasePtr BasePtr;
  typedef RBTreeNode<T> *Link_type;
  RBTree_iterator() : node() {}
  explicit RBTree_iterator(BasePtr x) : node(x) {}
  reference operator*() const {
    return *static_cast<Link_type>(node)->valPtr();
  }
  pointer operator->() const { return static_cast<Link_type>(node)->valPtr(); }
  RBT_it &operator++() {
    node = operation.increment(node);
    return *this;
  }
  RBT_it operator++(int) {
    RBT_it tmp = *this;
    node = operation.increment(node);
    return tmp;
  }
  RBT_it &operator--() {
    node = operation.decrement(node);
    return *this;
  }
  RBT_it operator--(int) {
    RBT_it tmp = *this;
    node = operation.decrement(node);
    return tmp;
  }
  bool operator==(const RBT_it &x) const { return node == x.node; }
  bool operator!=(const RBT_it &x) const { return node != x.node; }

  RBTree_operation_helper operation;
  BasePtr node;
};

template <typename T> struct RBTree_const_iterator {
  typedef T value_type;
  typedef const T &reference;
  typedef const T *pointer;
  typedef RBTree_iterator<T> iterator;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef RBTree_const_iterator<T> RBT_It;
  typedef RBTreeNodeBase::Const_BasePtr BasePtr;
  typedef const RBTreeNode<T> *Link_type;
  RBTree_const_iterator() : node() {}
  explicit RBTree_const_iterator(BasePtr x) : node(x) {}
  RBTree_const_iterator(const iterator &it) : node(it.node) {}
  iterator iterator_const_cast() const {
    return iterator(const_cast<typename iterator::BasePtr>(node));
  }
  reference operator*() const {
    return *static_cast<Link_type>(node)->valptr();
  }
  pointer operator->() const { return static_cast<Link_type>(node)->valptr(); }
  RBT_It &operator++() {
    node = operation.increment(node);
    return *this;
  }
  RBT_It operator++(int) {
    RBT_It tmp = *this;
    node = operation.increment(node);
    return tmp;
  }
  RBT_It &operator--() {
    node = operation.decrement(node);
    return *this;
  }
  RBT_It operator--(int) {
    RBT_It tmp = *this;
    node = operation.decrement(node);
    return tmp;
  }
  bool operator==(const RBT_It &x) const { return node == x.node; }
  bool operator!=(const RBT_It &x) const { return node != x.node; }

  RBTree_operation_helper operation;
  BasePtr node;
};

template <typename Val>
inline bool operator==(const RBTree_iterator<Val> &x,
                       const RBTree_const_iterator<Val> &y) {
  return x.node == y.node;
}
template <typename Val>
inline bool operator!=(const RBTree_iterator<Val> &x,
                       const RBTree_const_iterator<Val> &y) {
  return x.node != y.node;
}

///
/// @brief Red Black Tree class
///
/// @tparam Key
/// @tparam Value
/// @tparam KeyOfValue
/// @tparam Compare
/// @tparam Alloc
///
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
  typedef RBTree_iterator<value_type> iterator;
  typedef RBTree_const_iterator<value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

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

public:
  RBTree() {}

  RBTree(const Compare &comp, const allocator_type &a = allocator_type())
      : internalData(comp, node_allocator_type(a)) {}
  RBTree(const RBTree &x) : internalData(x.internalData) {
    if (x.root() != 0)
      root() = copy(x);
  }

  ~RBTree() { erase(begin()); }
  RBTree &operator=(const RBTree &x) {
    if (this != &x) {
      // Note that Key may be a constant type.
      ReuseOrAllocNode roan(*this);
      internalData.reset();
      internalData.keyCompare = x.internalData.keyCompare;
      if (x.root() != 0)
        root() = copy(x, roan);
    }
    return *this;
  }
  // Accessors.
  Compare key_comp() const { return internalData.keyCompare; }
  iterator begin() { return iterator(this->internalData.header.left); }
  const_iterator begin() const {
    return const_iterator(this->internalData.header.left);
  }
  iterator end() { return iterator(&this->internalData.header); }
  const_iterator end() const {
    return const_iterator(&this->internalData.header);
  }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  bool empty() const { return internalData.nodeCount == 0; }
  size_type size() const { return internalData.nodeCount; }
  size_type maxize() const { return (get_node_allocator_type().maxsize()); }
  void swap(RBTree &t) {
    if (root() == 0) {
      if (t.root() != 0)
        internalData.move_data(t.internalData);
    } else if (t.root() == 0)
      t.internalData.move_data(internalData);
    else {
      ft::swap(root(), t.root());
      ft::swap(leftmost(), t.leftmost());
      ft::swap(rightmost(), t.rightmost());
      root()->parent = end();
      t.root()->parent = t.end();
      ft::swap(this->internalData.nodeCount, t.internalData.nodeCount);
    }
    // No need to swap header's color as it does not change.
    ft::swap(this->internalData.keyCompare, t.internalData.keyCompare);
    ft::swap(get_node_allocator_type(), t.get_node_allocator_type());
  }
  // Insert/erase.

  void erase(iterator position) { erase_aux(position); }
  void erase(const_iterator position) { erase_aux(position); }
  size_type erase(const Key &x) {
    pair<iterator, iterator> p = equal_range(x);
    const size_type oldize = size();
    erase_aux(p.first, p.second);
    return oldize - size();
  }
  void erase(const Key *first, const Key *last) {
    while (first != last)
      erase(*first++);
  }
  void erase(iterator first, iterator last) { erase_aux(first, last); }
  void erase(const_iterator first, const_iterator last) {
    erase_aux(first, last);
  }
  void clear() {
    erase(begin());
    internalData.reset();
  }
  // Set operations.iterator
  iterator find(const Key &k) {
    iterator j = lower_bound(begin(), end(), k);
    return (j == end() || internalData.keyCompare(k, key(j.node))) ? end() : j;
  }
  const_iterator find(const Key &k) const {
    const_iterator j = lower_bound(begin(), end(), k);
    return (j == end() || internalData.keyCompare(k, key(j.node))) ? end() : j;
  }
  size_type count(const Key &k) const {
    pair<const_iterator, const_iterator> p = equal_range(k);
    const size_type n = ft::distance(p.first, p.second);
    return n;
  }
  iterator lower_bound(const key_type &k) {
    return lower_bound(begin(), end(), k);
  }
  const_iterator lower_bound(const key_type &k) const {
    return lower_bound(begin(), end(), k);
  }
  iterator upper_bound(const key_type &k) {
    return upper_bound(begin(), end(), k);
  }
  const_iterator upper_bound(const key_type &k) const {
    return upper_bound(begin(), end(), k);
  }

protected:
  BasePtr &root() { return internalData.header.parent; }
  Const_BasePtr root() const { return internalData.header.parent; }
  BasePtr &leftmost() { return internalData.header.left; }
  Const_BasePtr leftmost() const { return internalData.header.left; }
  BasePtr &rightmost() { return internalData.header.right; }
  Const_BasePtr rightmost() const { return internalData.header.right; }
  Link_type beginInternal() {
    return static_cast<Link_type>(internalData.header.parent);
  }
  Const_Link_type beginInternal() const {
    return static_cast<Const_Link_type>(internalData.header.parent);
  }
  BasePtr endInternal() { return &internalData.header; }
  Const_BasePtr endInternal() const { return &internalData.header; }
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

  template <typename NodeGen>
  iterator insert_(BasePtr x, BasePtr p,

                   const Value &v, NodeGen &node_gen) {
    bool insert_left = (x != 0 || p == end() ||
                        internalData.keyCompare(KeyOfValue()(v), key(p)));
    Link_type z = node_gen(v);
    insert_and_rebalance(insert_left, z, p, this->internalData.header);
    ++internalData.nodeCount;
    return iterator(z);
  }
  iterator

  insert_lower(BasePtr p, const Value &v)

  {
    bool insert_left =
        (p == end() || !internalData.keyCompare(key(p), KeyOfValue()(v)));
    Link_type z = create_node(v);
    insert_and_rebalance(insert_left, z, p, this->internalData.header);
    ++internalData.nodeCount;
    return iterator(z);
  }

  iterator insert_equal_lower(const Value &v)

  {
    Link_type x = begin();
    BasePtr y = end();
    while (x != 0) {
      y = x;
      x = !internalData.keyCompare(key(x), KeyOfValue()(v)) ? left(x)
                                                            : right(x);
    }
    return insert_lower(y, v);
  }

  template <typename NodeGen> Link_type copy(const RBTree &x, NodeGen &gen) {
    Link_type root = copy(x.begin(), end(), gen);
    leftmost() = minimum(root);
    rightmost() = maximum(root);
    internalData.nodeCount = x.internalData.nodeCount;
    return root;
  }
  Link_type copy(const RBTree &x) {
    AllocNode an(*this);
    return copy(x, an);
  }
  template <typename NodeGen>
  Link_type copy(Const_Link_type x, BasePtr p, NodeGen &node_gen) {
    // Structural copy. x and p must be non-null.
    Link_type top = clone_node(x, node_gen);
    top->parent = p;
    try {
      if (x->right)
        top->right = copy(right(x), top, node_gen);
      p = top;
      x = left(x);
      while (x != 0) {
        Link_type y = clone_node(x, node_gen);
        p->left = y;
        y->parent = p;
        if (x->right)
          y->right = copy(right(x), y, node_gen);
        p = y;
        x = left(x);
      }
    } catch (...) {
      erase(top);
      __throw_exception_again;
    }
    return top;
  }

  void erase(Link_type x) {
    // Erase without rebalancing.
    while (x != 0) {
      erase(right(x));
      Link_type y = left(x);
      drop_node(x);
      x = y;
    }
  }

  iterator lower_bound(Link_type x, BasePtr y, const Key &k) {
    while (x != 0)
      if (!internalData.keyCompare(key(x), k))
        y = x, x = left(x);
      else
        x = right(x);
    return iterator(y);
  }

  const_iterator lower_bound(Const_Link_type x, Const_BasePtr y,
                             const Key &k) const {
    while (x != 0)
      if (!internalData.keyCompare(key(x), k))
        y = x, x = left(x);
      else
        x = right(x);
    return const_iterator(y);
  }
  iterator upper_bound(Link_type x, BasePtr y, const Key &k) {
    while (x != 0)
      if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else
        x = right(x);
    return iterator(y);
  }
  const_iterator upper_bound(Const_Link_type x, Const_BasePtr y,
                             const Key &k) const {
    while (x != 0)
      if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else
        x = right(x);
    return const_iterator(y);
  }

  pair<iterator, iterator> equal_range(const Key &k) {
    Link_type x = begin();
    BasePtr y = end();
    while (x != 0) {
      if (internalData.keyCompare(key(x), k))
        x = right(x);
      else if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else {
        Link_type xu(x);
        BasePtr yu(y);
        y = x, x = left(x);
        xu = right(xu);
        return pair<iterator, iterator>(lower_bound(x, y, k),
                                        upper_bound(xu, yu, k));
      }
    }
    return pair<iterator, iterator>(iterator(y), iterator(y));
  }

  pair<const_iterator, const_iterator> equal_range(const Key &k) const {
    Const_Link_type x = begin();
    Const_BasePtr y = end();
    while (x != 0) {
      if (internalData.keyCompare(key(x), k))
        x = right(x);
      else if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else {
        Const_Link_type xu(x);
        Const_BasePtr yu(y);
        y = x, x = left(x);
        xu = right(xu);
        return pair<const_iterator, const_iterator>(lower_bound(x, y, k),
                                                    upper_bound(xu, yu, k));
      }
    }
    return pair<const_iterator, const_iterator>(const_iterator(y),
                                                const_iterator(y));
  }

  pair<BasePtr, BasePtr> get_insert_unique_pos(const key_type &k) {
    typedef pair<BasePtr, BasePtr> Res;
    Link_type x = begin();
    BasePtr y = end();
    bool comp = true;
    while (x != 0) {
      y = x;
      comp = internalData.keyCompare(k, key(x));
      x = comp ? left(x) : right(x);
    }
    iterator j = iterator(y);
    if (comp) {
      if (j == begin())
        return Res(x, y);
      else
        --j;
    }
    if (internalData.keyCompare(key(j.node), k))
      return Res(x, y);
    return Res(j.node, 0);
  }

  pair<BasePtr, BasePtr> get_insert_equal_pos(const key_type &k) {
    typedef pair<BasePtr, BasePtr> Res;
    Link_type x = begin();
    BasePtr y = end();
    while (x != 0) {
      y = x;
      x = internalData.keyCompare(k, key(x)) ? left(x) : right(x);
    }
    return Res(x, y);
  }
  pair<iterator, bool>

  insert_unique(const Value &v)

  {
    typedef pair<iterator, bool> Res;
    pair<BasePtr, BasePtr> res = get_insert_unique_pos(KeyOfValue()(v));
    if (res.second) {
      AllocNode an(*this);
      return Res(_insert(res.first, res.second, v, an), true);
    }
    return Res(iterator(res.first), false);
  }

  iterator insert_equal(const Value &v)

  {
    pair<BasePtr, BasePtr> res = get_insert_equal_pos(KeyOfValue()(v));
    AllocNode an(*this);
    return insert_(res.first, res.second, v, an);
  }

  pair<BasePtr, BasePtr> get_insert_hint_unique_pos(const_iterator position,
                                                    const key_type &k) {
    iterator pos = position.iterator_const_cast();
    typedef pair<BasePtr, BasePtr> Res;
    // end()
    if (pos.node == end()) {
      if (size() > 0 && internalData.keyCompare(key(rightmost()), k))
        return Res(0, rightmost());
      else
        return get_insert_unique_pos(k);
    } else if (internalData.keyCompare(k, key(pos.node))) {
      // First, try before...
      iterator before = pos;
      if (pos.node == leftmost()) // begin()
        return Res(leftmost(), leftmost());
      else if (internalData.keyCompare(key((--before).node), k)) {
        if (right(before.node) == 0)
          return Res(0, before.node);
        else
          return Res(pos.node, pos.node);
      } else
        return get_insert_unique_pos(k);
    } else if (internalData.keyCompare(key(pos.node), k)) {
      // ... then try after.
      iterator after = pos;
      if (pos.node == rightmost())
        return Res(0, rightmost());
      else if (internalData.keyCompare(k, key((++after).node))) {
        if (right(pos.node) == 0)
          return Res(0, pos.node);
        else
          return Res(after.node, after.node);
      } else
        return get_insert_unique_pos(k);
    } else
      // Equivalent keys.
      return Res(pos.node, 0);
  }
  template <typename NodeGen>
  iterator insert_unique_(const_iterator position,

                          const Value &v, NodeGen &node_gen) {
    pair<BasePtr, BasePtr> res =
        get_insert_hint_unique_pos(position, KeyOfValue()(v));
    if (res.second)
      return insert_(res.first, res.second, v, node_gen);
    return iterator(res.first);
  }
  pair<BasePtr, BasePtr> get_insert_hint_equal_pos(const_iterator position,
                                                   const key_type &k) {
    iterator pos = position.iterator_const_cast();
    typedef pair<BasePtr, BasePtr> Res;
    // end()
    if (pos.node == end()) {
      if (size() > 0 && !internalData.keyCompare(k, key(rightmost())))
        return Res(0, rightmost());
      else
        return get_insert_equal_pos(k);
    } else if (!internalData.keyCompare(key(pos.node), k)) {
      // First, try before...
      iterator before = pos;
      if (pos.node == leftmost()) // begin()
        return Res(leftmost(), leftmost());
      else if (!internalData.keyCompare(k, key((--before).node))) {
        if (right(before.node) == 0)
          return Res(0, before.node);
        else
          return Res(pos.node, pos.node);
      } else
        return get_insert_equal_pos(k);
    } else {
      // ... then try after.
      iterator after = pos;
      if (pos.node == rightmost())
        return Res(0, rightmost());
      else if (!internalData.keyCompare(key((++after).node), k)) {
        if (right(pos.node) == 0)
          return Res(0, pos.node);
        else
          return Res(after.node, after.node);
      } else
        return Res(0, 0);
    }
  }
  template <typename NodeGen>
  iterator insert_equal_(const_iterator position,

                         const Value &v, NodeGen &node_gen) {
    pair<BasePtr, BasePtr> res =
        get_insert_hint_equal_pos(position, KeyOfValue()(v));
    if (res.second)
      return insert_(res.first, res.second, v, node_gen);
    return insert_equal_lower(v);
  }

  template <class II> void insert_unique(II first, II last) {
    AllocNode an(*this);
    for (; first != last; ++first)
      insert_unique_(end(), *first, an);
  }

  template <class II> void insert_equal(II first, II last) {
    AllocNode an(*this);
    for (; first != last; ++first)
      insert_equal_(end(), *first, an);
  }

  void erase_aux(const_iterator position) {
    Link_type y = static_cast<Link_type>(rebalance_for_erase(
        const_cast<BasePtr>(position.node), this->internalData.header));
    drop_node(y);
    --internalData.nodeCount;
  }

  void erase_aux(const_iterator first, const_iterator last) {
    if (first == begin() && last == end())
      clear();
    else
      while (first != last)
        erase_aux(first++);
  }

  bool rb_verify() const {
    if (internalData.nodeCount == 0 || begin() == end())
      return internalData.nodeCount == 0 && begin() == end() &&
             this->internalData.header.left == end() &&
             this->internalData.header.right == end();
    unsigned int len = _Rb_tree_black_count(leftmost(), root());
    for (const_iterator it = begin(); it != end(); ++it) {
      Const_Link_type x = static_cast<Const_Link_type>(it.node);
      Const_Link_type L = _left(x);
      Const_Link_type R = _right(x);
      if (x->color == Red)
        if ((L && L->color == Red) || (R && R->color == Red))
          return false;
      if (L && internalData.keyCompare(key(x), key(L)))
        return false;
      if (R && internalData.keyCompare(key(R), key(x)))
        return false;
      if (!L && !R && _Rb_tree_black_count(x, root()) != len)
        return false;
    }
    if (leftmost() != RBTreeNodeBase::minimum(root()))
      return false;
    if (rightmost() != RBTreeNodeBase::maximum(root()))
      return false;
    return true;
  }
};

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator==(const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                       const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator<(const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                      const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator!=(const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                       const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return !(x == y);
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator>(const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                      const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return y < x;
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator<=(const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                       const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return !(y < x);
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator>=(const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                       const RBTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return !(x < y);
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline void swap(RBTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                 RBTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  x.swap(y);
}

} // namespace ft

#endif
