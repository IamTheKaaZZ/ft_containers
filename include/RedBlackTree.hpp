/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedBlackTree.hpp                                         :+:      :+: :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 15:13:00 by bcosters          #+#    #+#             */
/*   Updated: 2022/07/26 16:24:44 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

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
///    5. Root is always black.
///    6. New insertions are always red (can change to black after testing the
///    rules).
///

namespace ft {

enum RedBlackTreeColor { Red = false, Black = true };

///
/// @brief Node Base class + Node operations.
///
///
template <typename Value> struct RedBlackTreeNode {

  typedef RedBlackTreeNode *node_ptr;
  typedef const RedBlackTreeNode *const_node_ptr;

  Value value;
  RedBlackTreeColor color;
  node_ptr parent;
  node_ptr left;
  node_ptr right;

  RedBlackTreeNode()
      : value(), color(Red), parent(NULL), left(NULL), right(NULL) {}

  RedBlackTreeNode& operator=(const RedBlackTreeNode& rhs) {
    value =  rhs.value;
    color = rhs.color;
    parent = rhs.parent;
    left = rhs.left;
    right = rhs.right;
  }

  Value *valPtr() { return ft::addressof(value); }
  const Value *valPtr() const { return ft::addressof(value); }

  node_ptr minimum() { return minimum(this); }
  static node_ptr minimum(node_ptr x) {
    while (x->left != NULL) {
      x = x->left;
    }
    return x;
  }
  static const_node_ptr minimum(const_node_ptr x) {
    while (x->left != NULL) {
      x = x->left;
    }
    return x;
  }

  node_ptr maximum() { return maximum(this); }
  static node_ptr maximum(node_ptr x) {
    while (x->right != NULL) {
      x = x->right;
    }
    return x;
  }
  static const_node_ptr maximum(const_node_ptr x) {
    while (x->right != NULL) {
      x = x->right;
    }
    return x;
  }
  static node_ptr increment_helper(node_ptr x) throw() {
    if (x->right != 0) {
      x = x->right;
      while (x->left != 0)
        x = x->left;
    } else {
      node_ptr y = x->parent;
      while (x == y->right) {
        x = y;
        y = y->parent;
      }
      if (x->right != y)
        x = y;
    }
    return x;
  }

  ///
  /// @brief Increment over the tree.
  ///
  /// @param x
  /// @return RedBlackTreeNode*
  ///
  node_ptr increment(node_ptr x) throw() { return increment_helper(x); }
  const_node_ptr increment(const_node_ptr x) throw() {
    return increment_helper(const_cast<node_ptr>(x));
  }
  node_ptr increment() { increment(this); }

  static node_ptr decrement_helper(node_ptr x) throw() {
    if (x->color == Red && x->parent->parent == x)
      x = x->right;
    else if (x->left != 0) {
      node_ptr y = x->left;
      while (y->right != 0)
        y = y->right;
      x = y;
    } else {
      node_ptr y = x->parent;
      while (x == y->left) {
        x = y;
        y = y->parent;
      }
      x = y;
    }
    return x;
  }

  ///
  /// @brief Decrement over the tree.
  ///
  /// @param x
  /// @return RedBlackTreeNode*
  ///
  node_ptr decrement(node_ptr x) throw() { return decrement_helper(x); }
  const_node_ptr decrement(const_node_ptr x) throw() {
    return decrement_helper(const_cast<node_ptr>(x));
  }
  node_ptr decrement() { decrement(this); }

  static void rotate_left_helper(node_ptr const x, node_ptr &root) {
    node_ptr const y = x->right;
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

  ///
  /// @brief Rotate the tree to the left.
  ///
  /// @param x
  /// @param root
  ///
  void rotate_left(node_ptr const x, node_ptr &root) {
    rotate_left_helper(x, root);
  }

  static void rotate_right_helper(node_ptr const x, node_ptr &root) {
    node_ptr const y = x->left;
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

  ///
  /// @brief Rotate the tree to the right.
  ///
  /// @param x
  /// @param root
  ///
  void rotate_right(node_ptr const x, node_ptr &root) {
    rotate_right_helper(x, root);
  }

  ///
  /// @brief Insert a value and rebalance the tree accordingly.
  ///
  /// @param insert_left
  /// @param x
  /// @param p
  /// @param header
  ///
  void insert_and_rebalance(const bool insert_left, node_ptr x, node_ptr p,
                            RedBlackTreeNode &header) throw() {
    node_ptr &root = header.parent;
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
      node_ptr const xpp = x->parent->parent;
      if (x->parent == xpp->left) {
        node_ptr const y = xpp->right;
        if (y && y->color == Red) {
          x->parent->color = Black;
          y->color = Black;
          xpp->color = Red;
          x = xpp;
        } else {
          if (x == x->parent->right) {
            x = x->parent;
            rotate_left(x, root);
          }
          x->parent->color = Black;
          xpp->color = Red;
          rotate_right(xpp, root);
        }
      } else {
        node_ptr const y = xpp->left;
        if (y && y->color == Red) {
          x->parent->color = Black;
          y->color = Black;
          xpp->color = Red;
          x = xpp;
        } else {
          if (x == x->parent->left) {
            x = x->parent;
            rotate_right(x, root);
          }
          x->parent->color = Black;
          xpp->color = Red;
          rotate_left(xpp, root);
        }
      }
    }
    root->color = Black;
  }

  ///
  /// @brief Rebalance the tree to erase the node.
  ///
  /// @param z
  /// @param header
  /// @return RedBlackTreeNode*
  ///
  node_ptr rebalance_for_erase(node_ptr const z,
                               RedBlackTreeNode &header) throw() {
    node_ptr &root = header.parent;
    node_ptr &leftmost = header.left;
    node_ptr &rightmost = header.right;
    node_ptr y = z;
    node_ptr x = 0;
    node_ptr x_parent = 0;
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
      ft::swap(y->color, z->color);
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
          leftmost = minimum(x);
      }
      if (rightmost == z) {
        if (z->left == 0) // z->right must be null also
          rightmost = z->parent;
        // makes rightmost == header if z == root
        else // x == z->left
          rightmost = maximum(x);
      }
    }
    if (y->color != Red) {
      while (x != root && (x == 0 || x->color == Black))
        if (x == x_parent->left) {
          node_ptr w = x_parent->right;
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
          node_ptr w = x_parent->left;
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

  ///
  /// @brief Count the black nodes between the node and root.
  ///
  /// @param node
  /// @param root
  /// @return unsigned int
  ///
  unsigned int black_count(const_node_ptr node, const_node_ptr root) throw() {
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

///
/// @brief Helper struct for the comparison of keys.
///
/// @tparam KeyCompare
///
template <typename KeyCompare> struct RedBlackTreeKeyCompare {

  KeyCompare keyCompare;

  RedBlackTreeKeyCompare() : keyCompare() {}
  RedBlackTreeKeyCompare(const KeyCompare &comp) : keyCompare(comp) {}
  RedBlackTreeKeyCompare(const RedBlackTreeKeyCompare &other)
      : keyCompare(other.keyCompare) {}
};

///
/// @brief Helper struct for default initialization.
///
///
template <typename Value> struct RedBlackTreeHeader {
  RedBlackTreeNode<Value> header;
  size_t nodeCount; // Keeps track of tree size

  // New nodes are always Red.
  RedBlackTreeHeader() {
    header.color = Red;
    reset();
  }

  void reset() {
    header.parent = NULL;
    header.left = &header;
    header.right = &header;
    nodeCount = 0;
  }

  void moveData(RedBlackTreeHeader &other) {
    header.color = other.header.color;
    header.parent = other.header.parent;
    header.left = other.header.left;
    header.right = other.header.right;
    header.parent->parent = &header;
    nodeCount = other.nodeCount;
  }
};

///
/// @brief Custom iterators for the RedBlackTree.
///
/// @tparam T
///
template <typename T> struct RedBlackTree_iterator {
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef RedBlackTree_iterator<T> RBT_it;
  typedef RedBlackTreeNode<T> *node_ptr;
  RedBlackTree_iterator() : node() {}
  explicit RedBlackTree_iterator(node_ptr x) : node(x) {}
  reference operator*() const { return *node->valPtr(); }
  pointer operator->() const { return node->valPtr(); }
  RBT_it &operator++() {
    node->increment();
    return *this;
  }
  RBT_it operator++(int) {
    RBT_it tmp = *this;
    node->increment();
    return tmp;
  }
  RBT_it &operator--() {
    node->decrement();
    return *this;
  }
  RBT_it operator--(int) {
    RBT_it tmp = *this;
    node->decrement();
    return tmp;
  }
  bool operator==(const RBT_it &x) const { return node == x.node; }
  bool operator!=(const RBT_it &x) const { return node != x.node; }

  node_ptr node;
};

template <typename T> struct RedBlackTree_const_iterator {
  typedef T value_type;
  typedef const T &reference;
  typedef const T *pointer;
  typedef RedBlackTree_iterator<T> iterator;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef RedBlackTree_const_iterator<T> RBT_It;
  typedef const RedBlackTreeNode<T> *node_ptr;
  RedBlackTree_const_iterator() : node() {}
  explicit RedBlackTree_const_iterator(node_ptr x) : node(x) {}
  RedBlackTree_const_iterator(const iterator &it) : node(it.node) {}
  iterator iterator_const_cast() const {
    return iterator(const_cast<typename iterator::node_ptr>(node));
  }
  reference operator*() const { return *node->valptr(); }
  pointer operator->() const { return node->valptr(); }
  RBT_It &operator++() {
    node->increment();
    return *this;
  }
  RBT_It operator++(int) {
    RBT_It tmp = *this;
    node->increment();
    return tmp;
  }
  RBT_It &operator--() {
    node->decrement();
    return *this;
  }
  RBT_It operator--(int) {
    RBT_It tmp = *this;
    node->decrement();
    return tmp;
  }
  bool operator==(const RBT_It &x) const { return node == x.node; }
  bool operator!=(const RBT_It &x) const { return node != x.node; }

  node_ptr node;
};

template <typename Val>
inline bool operator==(const RedBlackTree_iterator<Val> &x,
                       const RedBlackTree_const_iterator<Val> &y) {
  return x.node == y.node;
}
template <typename Val>
inline bool operator!=(const RedBlackTree_iterator<Val> &x,
                       const RedBlackTree_const_iterator<Val> &y) {
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
class RedBlackTree {
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
  typedef std::allocator<RedBlackTreeNode<Value>> node_allocator_type;

  ///
  /// @brief Get the node allocator type object
  ///
  /// @return node_allocator_type&
  ///
  node_allocator_type &get_node_allocator_type() { return internalData; }
  const node_allocator_type &get_node_allocator_type() const {
    return internalData;
  }

  ///
  /// @brief Get the allocator object
  ///
  /// @return allocator_type
  ///
  allocator_type get_allocator() const {
    return allocator_type(get_node_allocator_type());
  }
  typedef RedBlackTree_iterator<value_type> iterator;
  typedef RedBlackTree_const_iterator<value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

protected:
  typedef RedBlackTreeNode<Value> node;
  typedef node *node_ptr;
  typedef const node *const_node_ptr;

  ///
  /// @brief Get the node object -> allocate a new node.
  ///
  /// @returnnode_ptr
  ///
  node_ptr get_node() { return get_node_allocator_type().allocate(1); }

  ///
  /// @brief Deallocates a node object.
  ///
  /// @param p
  ///
  void put_node(node_ptr p) {
    get_node_allocator_type().deallocate(get_node_allocator_type(), p, 1);
  }

  ///
  /// @brief Construct a given node with a value.
  ///
  /// @param node
  /// @param x
  ///
  void construct_node(node_ptr node, const value_type &x) {
    get_allocator().construct(node->valPtr(), x);
  }

  ///
  /// @brief Create a node object: allocate and construct it.
  ///
  /// @param x
  /// @returnnode_ptr
  ///
  node_ptr create_node(const value_type &x) {
    node_ptr tmp = get_node();
    construct_node(tmp, x);
    return tmp;
  }

  ///
  /// @brief Destroy a node, does not free memory.
  ///
  /// @param p
  ///
  void destroy_node(node_ptr p) { get_allocator().destroy(p->valPtr()); }

  ///
  /// @brief Destroy and deallocate a node.
  ///
  /// @param p
  ///
  void drop_node(node_ptr p) {
    destroy_node(p);
    put_node(p);
  }

  ///
  /// @brief Clone a node object.
  ///
  /// @tparam NodeGen
  /// @param x
  /// @param node_gen
  /// @returnnode_ptr
  ///
  template <typename NodeGen>
  node_ptr clone_node(const_node_ptr x, NodeGen &node_gen) {
    node_ptr tmp = node_gen(*x->valPtr());
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
  }

  ///
  /// @brief Internal data struct for the RedBlackTree, holds the node
  /// allocator, the comparison and the header.
  ///
  /// @tparam KeyCompare
  ///
  template <typename KeyCompare>
  struct RedBlackTreeInternal : public node_allocator_type,
                                public RedBlackTreeKeyCompare<KeyCompare>,
                                public RedBlackTreeHeader<Value> {
    RedBlackTreeInternal() : node_allocator_type() {}
    RedBlackTreeInternal(const RedBlackTreeInternal &other)
        : node_allocator_type(other), RedBlackTreeKeyCompare<KeyCompare>(
                                          other.keyCompare) {}
    RedBlackTreeInternal(const KeyCompare &comp, const node_allocator_type &a)
        : node_allocator_type(a), RedBlackTreeKeyCompare<KeyCompare>(comp) {}
  };

  RedBlackTreeInternal<Compare> internalData;

private: // Functors
  ///
  /// @brief Functor that recycles a pool of nodes and uses allocation once a
  /// pool is empty.
  ///
  struct ReuseOrAllocNode {
    ReuseOrAllocNode(RedBlackTree &t)
        : rootNode(t.root()), nodes(t.rightmost()), tree(t) {
      if (rootNode) {
        rootNode->parent = 0;
        if (nodes->left)
          nodes = nodes->left;
      } else
        nodes = 0;
    }

    ~ReuseOrAllocNode() { tree.erase(static_cast<node_ptr>(rootNode)); }
    template <typename Arg> node_ptr operator()(const Arg &arg) {
      node_ptr node = static_cast<node_ptr>(extract());
      if (node) {
        tree.destroy_node(node);
        tree.construct_node(node, arg);
        return node;
      }
      return tree.create_node(arg);
    }

    node_ptr extract() {
      if (!nodes)
        return nodes;
      node_ptr node = nodes;
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
    node_ptr rootNode;
    node_ptr nodes;
    RedBlackTree &tree;
  };

  ///
  /// @brief Functor that doesn't recycle a pool of nodes.
  ///
  ///
  struct AllocNode {
    AllocNode(RedBlackTree &t) : tree(t) {}
    template <typename Arg> node_ptr operator()(const Arg &arg) const {
      return tree.create_node(arg);
    }

  private:
    RedBlackTree &tree;
  };

public:
  RedBlackTree() {}
  RedBlackTree(const Compare &comp, const allocator_type &a = allocator_type())
      : internalData(comp, node_allocator_type(a)) {}
  RedBlackTree(const RedBlackTree &x) : internalData(x.internalData) {
    if (x.root() != 0)
      root() = copy(x);
  }
  ~RedBlackTree() { erase(begin()); }

  ///
  /// @brief Assignment operator.
  ///
  /// @param x
  /// @return RedBlackTree&
  ///
  RedBlackTree &operator=(const RedBlackTree &x) {
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
  void swap(RedBlackTree &t) {
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

  ///
  /// @brief Creates the key pair if it doesn't exist.
  ///
  /// @param v
  /// @return pair<iterator, bool>
  ///
  pair<iterator, bool> insert_unique(const Value &v) {
    typedef pair<iterator, bool> Res;
    pair<node_ptr, node_ptr> res = get_insert_unique_pos(KeyOfValue()(v));
    if (res.second) {
      AllocNode an(*this);
      return Res(insert_(res.first, res.second, v, an), true);
    }
    return Res(iterator(res.first), false);
  }

  ///
  /// @brief Finds the equal key to insert the new node in.
  ///
  /// @param v
  /// @return iterator
  ///
  iterator insert_equal(const Value &v) {
    pair<node_ptr, node_ptr> res = get_insert_equal_pos(KeyOfValue()(v));
    AllocNode an(*this);
    return insert_(res.first, res.second, v, an);
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

  ///
  /// @brief Erase the position from the tree.
  ///
  /// @param position
  ///
  void erase(iterator position) { erase_aux(position); }
  void erase(const_iterator position) { erase_aux(position); }

  ///
  /// @brief Erase all keys of the same value.
  ///
  /// @param x
  /// @return size_type
  ///
  size_type erase(const Key &x) {
    pair<iterator, iterator> p = equal_range(x);
    const size_type oldsize = size();
    erase_aux(p.first, p.second);
    return oldsize - size();
  }

  ///
  /// @brief Erase a range of keys.
  ///
  /// @param first
  /// @param last
  ///
  void erase(const Key *first, const Key *last) {
    while (first != last)
      erase(*first++);
  }

  ///
  /// @brief Erase a range of values.
  ///
  /// @param first
  /// @param last
  ///
  void erase(iterator first, iterator last) { erase_aux(first, last); }
  void erase(const_iterator first, const_iterator last) {
    erase_aux(first, last);
  }

  ///
  /// @brief Empty the tree.
  ///
  ///
  void clear() {
    erase_(begin());
    internalData.reset();
  }

  // Set operations

  ///
  /// @brief Find by key.
  ///
  /// @param k
  /// @return iterator
  ///
  iterator find(const Key &k) {
    iterator j = lower_bound(begin(), end(), k);
    return (j == end() || internalData.keyCompare(k, key(j.node))) ? end() : j;
  }
  const_iterator find(const Key &k) const {
    const_iterator j = lower_bound(begin(), end(), k);
    return (j == end() || internalData.keyCompare(k, key(j.node))) ? end() : j;
  }

  ///
  /// @brief Count how many times the same key is used.
  ///
  /// @param k
  /// @return size_type
  ///
  size_type count(const Key &k) const {
    pair<const_iterator, const_iterator> p = equal_range(k);
    const size_type n = ft::distance(p.first, p.second);
    return n;
  }

  ///
  /// @brief Find the lower_bound of the given key in the tree (earliest it
  /// appears).
  ///
  /// @param k
  /// @return iterator
  ///
  iterator lower_bound(const key_type &k) {
    return lower_bound_(begin(), end(), k);
  }
  const_iterator lower_bound(const key_type &k) const {
    return lower_bound_(begin(), end(), k);
  }

  ///
  /// @brief Find the upper_bound of the given key (latest it appears).
  ///
  /// @param k
  /// @return iterator
  ///
  iterator upper_bound(const key_type &k) {
    return upper_bound_(begin(), end(), k);
  }
  const_iterator upper_bound(const key_type &k) const {
    return upper_bound_(begin(), end(), k);
  }

protected:
  node_ptr &root() { return internalData.header.parent; }
  const_node_ptr root() const { return internalData.header.parent; }
  node_ptr &leftmost() { return internalData.header.left; }
  const_node_ptr leftmost() const { return internalData.header.left; }
  node_ptr &rightmost() { return internalData.header.right; }
  const_node_ptr rightmost() const { return internalData.header.right; }
  node_ptr beginInternal() {
    return static_cast<node_ptr>(internalData.header.parent);
  }
  const_node_ptr beginInternal() const {
    return static_cast<const_node_ptr>(internalData.header.parent);
  }
  node_ptr endInternal() { return &internalData.header; }
  const_node_ptr endInternal() const { return &internalData.header; }
  static const_reference value(const_node_ptr x) { return *x->valptr(); }
  static const Key &key(const_node_ptr x) { return KeyOfValue()(value(x)); }
  static node_ptr left(node_ptr x) { return static_cast<node_ptr>(x->left); }
  static const_node_ptr left(const_node_ptr x) {
    return static_cast<const_node_ptr>(x->left);
  }
  static node_ptr right(node_ptr x) { return static_cast<node_ptr>(x->right); }
  static const_node_ptr right(const_node_ptr x) {
    return static_cast<const_node_ptr>(x->right);
  }
  static node_ptr minimum(node_ptr x) { return x->minimum(); }
  static const_node_ptr minimum(const_node_ptr x) { return x->minimum(); }
  static node_ptr maximum(node_ptr x) { return x->maximum(); }
  static const_node_ptr maximum(const_node_ptr x) { return x->maximum(); }

  ///
  /// @brief Insert the value with a node generator.
  ///
  /// @tparam NodeGen
  /// @param x
  /// @param p
  /// @param v
  /// @param node_gen
  /// @return iterator
  ///
  template <typename NodeGen>
  iterator insert_(node_ptr x, node_ptr p, const Value &v, NodeGen &node_gen) {
    bool insert_left = (x != 0 || p == end() ||
                        internalData.keyCompare(KeyOfValue()(v), key(p)));
    node_ptr z = node_gen(v);
    insert_and_rebalance(insert_left, z, p, this->internalData.header);
    ++internalData.nodeCount;
    return iterator(z);
  }

  ///
  /// @brief Insert the value in the lower_bound.
  ///
  /// @param p
  /// @param v
  /// @return iterator
  ///
  iterator insert_lower(node_ptr p, const Value &v) {
    bool insert_left =
        (p == end() || !internalData.keyCompare(key(p), KeyOfValue()(v)));
    node_ptr z = create_node(v);
    insert_and_rebalance(insert_left, z, p, this->internalData.header);
    ++internalData.nodeCount;
    return iterator(z);
  }

  ///
  /// @brief Insert the value in the equal lower_bound.
  ///
  /// @param v
  /// @return iterator
  ///
  iterator insert_equal_lower(const Value &v) {
    node_ptr x = begin();
    node_ptr y = end();
    while (x != 0) {
      y = x;
      x = !internalData.keyCompare(key(x), KeyOfValue()(v)) ? left(x)
                                                            : right(x);
    }
    return insert_lower(y, v);
  }

  ///
  /// @brief Copy a RedBlackTree with a Node Generator.
  ///
  /// @tparam NodeGen
  /// @param x
  /// @param gen
  /// @return node_ptr
  ///
  template <typename NodeGen>
  node_ptr copy(const RedBlackTree &x, NodeGen &gen) {
    node_ptr root = copy(x.begin(), end(), gen);
    leftmost() = minimum(root);
    rightmost() = maximum(root);
    internalData.nodeCount = x.internalData.nodeCount;
    return root;
  }
  node_ptr copy(const RedBlackTree &x) {
    AllocNode an(*this);
    return copy(x, an);
  }
  template <typename NodeGen>
  node_ptr copy(const_node_ptr x, node_ptr p, NodeGen &node_gen) {
    // Structural copy. x and p must be non-null.
    node_ptr top = clone_node(x, node_gen);
    top->parent = p;
    try {
      if (x->right)
        top->right = copy(right(x), top, node_gen);
      p = top;
      x = left(x);
      while (x != 0) {
        node_ptr y = clone_node(x, node_gen);
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

  ///
  /// @brief Erase all nodes from this one down without rebalancing the tree.
  ///
  /// @param x
  ///
  void erase_(node_ptr x) {
    // Erase without rebalancing.
    while (x != 0) {
      erase(right(x));
      node_ptr y = left(x);
      drop_node(x);
      x = y;
    }
  }

  ///
  /// @brief Find the lower_bound of x and y by going left when the keys are not
  /// equal and right when they are.
  ///
  /// @param x
  /// @param y
  /// @param k
  /// @return iterator
  ///
  iterator lower_bound_(node_ptr x, node_ptr y, const Key &k) {
    while (x != 0)
      if (!internalData.keyCompare(key(x), k))
        y = x, x = left(x);
      else
        x = right(x);
    return iterator(y);
  }
  const_iterator lower_bound_(const_node_ptr x, const_node_ptr y,
                              const Key &k) const {
    while (x != 0)
      if (!internalData.keyCompare(key(x), k))
        y = x, x = left(x);
      else
        x = right(x);
    return const_iterator(y);
  }

  ///
  /// @brief Find the upper_bound of x and y by going left when the keys are
  /// equal and right when they aren't.
  ///
  /// @param x
  /// @param y
  /// @param k
  /// @return iterator
  ///
  iterator upper_bound_(node_ptr x, node_ptr y, const Key &k) {
    while (x != 0)
      if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else
        x = right(x);
    return iterator(y);
  }
  const_iterator upper_bound_(const_node_ptr x, const_node_ptr y,
                              const Key &k) const {
    while (x != 0)
      if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else
        x = right(x);
    return const_iterator(y);
  }

  ///
  /// @brief Find a subset where all elements have the given Key.
  ///
  /// @param k
  /// @return pair<iterator, iterator>
  ///
  pair<iterator, iterator> equal_range(const Key &k) {
    node_ptr x = begin();
    node_ptr y = end();
    while (x != 0) {
      if (internalData.keyCompare(key(x), k))
        x = right(x);
      else if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else {
        node_ptr xu(x);
        node_ptr yu(y);
        y = x, x = left(x);
        xu = right(xu);
        return pair<iterator, iterator>(lower_bound(x, y, k),
                                        upper_bound(xu, yu, k));
      }
    }
    return pair<iterator, iterator>(iterator(y), iterator(y));
  }

  pair<const_iterator, const_iterator> equal_range(const Key &k) const {
    const_node_ptr x = begin();
    const_node_ptr y = end();
    while (x != 0) {
      if (internalData.keyCompare(key(x), k))
        x = right(x);
      else if (internalData.keyCompare(k, key(x)))
        y = x, x = left(x);
      else {
        const_node_ptr xu(x);
        const_node_ptr yu(y);
        y = x, x = left(x);
        xu = right(xu);
        return pair<const_iterator, const_iterator>(lower_bound(x, y, k),
                                                    upper_bound(xu, yu, k));
      }
    }
    return pair<const_iterator, const_iterator>(const_iterator(y),
                                                const_iterator(y));
  }

  ///
  /// @brief Get the unique position to insert the new node.
  ///
  /// @param k
  /// @return pair<node_ptr, node_ptr>
  ///
  pair<node_ptr, node_ptr> get_insert_unique_pos(const key_type &k) {
    typedef pair<node_ptr, node_ptr> Res;
    node_ptr x = begin();
    node_ptr y = end();
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
    /// Key already exists.
    return Res(j.node, NULL);
  }

  ///
  /// @brief Get the equal postion to insert the new node.
  ///
  /// @param k
  /// @return pair<node_ptr, node_ptr>
  ///
  pair<node_ptr, node_ptr> get_insert_equal_pos(const key_type &k) {
    typedef pair<node_ptr, node_ptr> Res;
    node_ptr x = begin();
    node_ptr y = end();
    while (x != 0) {
      y = x;
      x = internalData.keyCompare(k, key(x)) ? left(x) : right(x);
    }
    return Res(x, y);
  }

  pair<node_ptr, node_ptr> get_insert_hint_unique_pos(const_iterator position,
                                                      const key_type &k) {
    iterator pos = position.iterator_const_cast();
    typedef pair<node_ptr, node_ptr> Res;
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
  iterator insert_unique_(const_iterator position, const Value &v,
                          NodeGen &node_gen) {
    pair<node_ptr, node_ptr> res =
        get_insert_hint_unique_pos(position, KeyOfValue()(v));
    if (res.second)
      return insert_(res.first, res.second, v, node_gen);
    return iterator(res.first);
  }

  pair<node_ptr, node_ptr> get_insert_hint_equal_pos(const_iterator position,
                                                     const key_type &k) {
    iterator pos = position.iterator_const_cast();
    typedef pair<node_ptr, node_ptr> Res;
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
    pair<node_ptr, node_ptr> res =
        get_insert_hint_equal_pos(position, KeyOfValue()(v));
    if (res.second)
      return insert_(res.first, res.second, v, node_gen);
    return insert_equal_lower(v);
  }

  ///
  /// @brief Rebalance the tree for erasure of thew given node.
  ///
  /// @param position
  ///
  void erase_aux(const_iterator position) {
    node_ptr y = static_cast<node_ptr>(rebalance_for_erase(
        const_cast<node_ptr>(position.node), this->internalData.header));
    drop_node(y);
    --internalData.nodeCount;
  }

  ///
  /// @brief Erase a range.
  ///
  /// @param first
  /// @param last
  ///
  void erase_aux(const_iterator first, const_iterator last) {
    if (first == begin() && last == end())
      clear();
    else
      while (first != last)
        erase_aux(first++);
  }

  ///
  /// @brief Verify if the RedBlackTree is according to the rules.
  ///
  /// @return true
  /// @return false
  ///
  bool rb_verify() const {
    if (internalData.nodeCount == 0 || begin() == end())
      return internalData.nodeCount == 0 && begin() == end() &&
             this->internalData.header.left == end() &&
             this->internalData.header.right == end();
    unsigned int len = node::black_count(leftmost(), root());
    for (const_iterator it = begin(); it != end(); ++it) {
      const_node_ptr x = static_cast<const_node_ptr>(it.node);
      const_node_ptr L = left(x);
      const_node_ptr R = right(x);
      // Check if the children of a red node are black.
      if (x->color == Red)
        if ((L && L->color == Red) || (R && R->color == Red))
          return false;
      // Check that Left and Right have different values.
      if (L && internalData.keyCompare(key(x), key(L)))
        return false;
      if (R && internalData.keyCompare(key(R), key(x)))
        return false;
      // Check if the amount of black nodes is correct.
      if (!L && !R && node::black_count(x, root()) != len)
        return false;
    }
    // Check if the leftmost is the minimum node / rightmost is the maximum
    // node.
    if (leftmost() != root()->minimum())
      return false;
    if (rightmost() != root()->maximum())
      return false;
    return true;
  }
};

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool
operator==(const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
           const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool
operator<(const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
          const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool
operator!=(const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
           const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return !(x == y);
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool
operator>(const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
          const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return y < x;
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool
operator<=(const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
           const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return !(y < x);
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool
operator>=(const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
           const RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  return !(x < y);
}
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline void swap(RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &x,
                 RedBlackTree<Key, Val, KeyOfValue, Compare, Alloc> &y) {
  x.swap(y);
}

} // namespace ft

#endif
