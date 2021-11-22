/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include "macro.h"
#include "rbtree.h"

static __always_inline
void child_change(struct rb_node *old, struct rb_node *new,
                  struct rb_node *parent, struct rb_root *root)
{
    if (!parent)
        root->rb_node = new;
    else if (parent->left == old)
        parent->left = new;
    else
        parent->right = new;
}

static __always_inline
void rotate_set(struct rb_node *old, struct rb_node *new,
                bool color, struct rb_root *root)
{
    struct rb_node *parent = old->parent;

    new->parent = old->parent;
    new->color = old->color;
    old->parent = new;
    old->color = color;

    child_change(old, new, parent, root);
}

/**
 * left_rotate - left rotation one rb node.
 * @node: node to rotation.
 * @color: color after rotation.
 * @root: rbtree root of node.
 */
static __always_inline void
left_rotate(struct rb_node *node, bool color, bool ccolor, struct rb_root *root)
{
    struct rb_node *child, *parent = node->right;

    /* change left child */
    child = node->right = parent->left;
    if (child) {
        child->parent = node;
        child->color = ccolor;
    }

    parent->left = node;
    rotate_set(node, parent, color, root);
}

/**
 * right_rotate - right rotation one rb node.
 * @node: node to rotation.
 * @color: color after rotation.
 * @root: rbtree root of node.
 */
static __always_inline void
right_rotate(struct rb_node *node, bool color, bool ccolor, struct rb_root *root)
{
    struct rb_node *child, *parent = node->left;

    /* change right child */
    child = node->left = parent->right;
    if (child) {
        child->parent = node;
        child->color = ccolor;
    }

    parent->right = node;
    rotate_set(node, parent, color, root);
}

/**
 * rb_fixup - balance newly inserted nodes.
 * @root: rbtree root of node.
 * @node: new inserted node.
 */
void rb_fixup(struct rb_root *root, struct rb_node *node)
{
    struct rb_node *parent, *gparent, *tmp;

    while (root && node) {
        parent = node->parent;

        /* If node is root */
        if (unlikely(!parent)) {
            node->color = RB_BLACK;
            break;
        }

        /* If there is a black parent, we are done. */
        if (parent->color == RB_BLACK)
            break;

        gparent = parent->parent;
        tmp = gparent->right;

        if (tmp != parent) {
            /*
             * Case 1 - node's uncle is red (color flips).
             *
             *       G            g
             *      / \          / \
             *     p   t  -->   P   T
             *    /            /
             *   n            n
             *
             * However, since g's parent might be red, and
             * 4) does not allow this, we need to recurse
             * at g.
             */

            if (tmp && tmp->color == RB_RED) {
                parent->color = tmp->color = RB_BLACK;
                gparent->color = RB_RED;
                node = gparent;
                continue;
            }

            /*
             * Case 2 - node's uncle is black and node is
             * the parent's right child (left rotate at parent).
             *
             *      G             G
             *     / \           / \
             *    p   U  -->    n   U
             *     \           /
             *      n         p
             *     /           \
             *    t             T
             *
             * This still leaves us in violation of 4), the
             * continuation into Case 3 will fix that.
             */

            if (node == parent->right)
                left_rotate(parent, RB_RED, RB_BLACK, root);

            /*
             * Case 3 - node's uncle is black and node is
             * the parent's left child (right rotate at gparent).
             *
             *        G           P
             *       / \         / \
             *      p   U  -->  n   g
             *     / \             / \
             *    n   t           T   U
             */

            right_rotate(gparent, RB_RED, RB_BLACK, root);
            break;
        } else {
            /* parent == gparent->right */
            tmp = gparent->left;

            /* Case 1 - color flips */
            if (tmp && tmp->color == RB_RED) {
                parent->color = tmp->color = RB_BLACK;
                gparent->color = RB_RED;
                node = gparent;
                continue;
            }

            /* Case 2 - right rotate at parent */
            if (node == parent->left)
                right_rotate(parent, RB_RED, RB_BLACK, root);

            /* Case 3 - left rotate at gparent */
            left_rotate(gparent, RB_RED, RB_BLACK, root);
            break;
        }
    }
}

void rb_del(const struct rb_root *root, struct rb_node *node)
{
    while (root && node) {

    }
}

/**
 * left_far - go left as we can.
 * @node: node to go left.
 */
static __always_inline
struct rb_node *left_far(const struct rb_node *node)
{
    while (node->left)
        node = node->left;

    return (struct rb_node *)node;
}

/**
 * right_far - go right as we can.
 * @node: node to go right.
 */
static __always_inline
struct rb_node *right_far(const struct rb_node *node)
{
    while (node->right)
        node = node->right;

    return (struct rb_node *)node;
}

/**
 * left_deep - go left deep as we can.
 * @node: node to go left deep.
 */
static __always_inline
struct rb_node *left_deep(const struct rb_node *node)
{
    while (node) {
        if (node->left)
            node = node->left;
        else if (node->right)
            node = node->right;
        else
            return (struct rb_node *)node;
    }

    return NULL;
}

/**
 * right_deep - go right deep as we can.
 * @node: node to go right deep.
 */
static __always_inline
struct rb_node *right_deep(const struct rb_node *node)
{
    while (node) {
        if (node->right)
            node = node->right;
        else if (node->left)
            node = node->left;
        else
            return (struct rb_node *)node;
    }

    return NULL;
}

/**
 * rb_first/last/prev/next - Middle iteration (Sequential)
 * NOTE: find logical next and previous nodes
 */
struct rb_node *rb_first(const struct rb_root *root)
{
    struct rb_node *node = root->rb_node;

    if (!root || !node)
        return NULL;

    /* Get the leftmost node */
    node = left_far(node);
    return node;
}

struct rb_node *rb_last(const struct rb_root *root)
{
    struct rb_node *node = root->rb_node;

    if (!root || !node)
        return NULL;

    /* Get the rightmost node */
    node = right_far(node);
    return node;
}

struct rb_node *rb_prev(const struct rb_node *node)
{
    struct rb_node *parent;

    if (!node)
        return NULL;

    /*
     * If there is a left-hand node, go down
     * and then as far right as possible.
     */
    if (node->left) {
        node = node->left;
        return right_far(node);
    }

    /*
     * No left-hand children. Go up till we find an ancestor
     * which is a right-hand child of its parent.
     */
    while ((parent = node->parent) && node != parent->right)
        node = parent;

    return parent;
}

struct rb_node *rb_next(const struct rb_node *node)
{
    struct rb_node *parent;

    if (!node)
        return NULL;

    /*
     * If there is a right-hand node, go down
     * and then as far left as possible.
     */
    if (node->right) {
        node = node->right;
        return left_far(node);
    }

    /*
     * No right-hand children. Go up till we find an ancestor
     * which is a left-hand child of its parent.
     */
    while ((parent = node->parent) && node != parent->left)
        node = parent;

    return parent;
}

/**
 * rb_post_first/next - Postorder iteration (Depth-first)
 * NOTE: always visit the parent after its children
 */
struct rb_node *rb_post_first(const struct rb_root *root)
{
    struct rb_node *node = root->rb_node;

    if (!root || !node)
        return NULL;

    node = left_deep(node);
    return node;
}

struct rb_node *rb_post_next(const struct rb_node *node)
{
    const struct rb_node *parent;

    if (!node)
        return NULL;

    parent = node->parent;

    if (parent && node == parent->left && parent->right)
        return left_deep(parent->right);
    else
        return (struct rb_node *)parent;
}
