/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>
#include <stdbool.h>
#include <errno.h>

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define RB_RED      (0)
#define RB_BLACK    (1)
#define RB_NSET     (2)

typedef struct rb_node {
    struct rb_node *parent;
    struct rb_node *left;
    struct rb_node *right;
    bool color;
} rb_node_t;

struct rb_root {
    struct rb_node *rb_node;
};

struct rb_root_cached {
    struct rb_root root;
    struct rb_node *rb_leftmost;
};

#define RB_ROOT(name) \
    struct rb_root name = {NULL};

#define RB_ROOT_CACHED(name) \
    struct rb_root_cached name = {{NULL}, NULL};

#define rb_entry(ptr, type, member) ({                  \
    const typeof(((type *)0)->member) *__mptr = (ptr);  \
    (type *)((char *)__mptr - offsetof(type,member));   \
})

#define rb_entry_safe(ptr, type, member) ({             \
    typeof(ptr) _ptr = (ptr);                           \
    _ptr ? rb_entry(_ptr, type, member) : NULL;         \
})

typedef long (*rb_find_t)(const struct rb_node *, const void *key);
typedef long (*rb_cmp_t)(const struct rb_node *, const struct rb_node *);

extern void rb_fixup(struct rb_root *root, struct rb_node *node);
extern void rb_erase(struct rb_root *root, struct rb_node *parent);
extern struct rb_node *rb_remove(struct rb_root *root, struct rb_node *node);
extern void rb_replace(struct rb_root *root, struct rb_node *old, struct rb_node *new);
extern struct rb_node *rb_find(const struct rb_root *root, const void *key, rb_find_t);
extern struct rb_node **rb_parent(struct rb_root *root, struct rb_node **parentp, struct rb_node *node, rb_cmp_t, bool *leftmost);

/* Middle iteration (Sequential) - find logical next and previous nodes */
extern struct rb_node *rb_first(const struct rb_root *root);
extern struct rb_node *rb_last(const struct rb_root *root);
extern struct rb_node *rb_prev(const struct rb_node *node);
extern struct rb_node *rb_next(const struct rb_node *node);

#define rb_first_entry(ptr, type, member) \
    rb_entry_safe(rb_first(ptr), type, member)

#define rb_next_entry(pos, member) \
    rb_entry_safe(rb_next(&(pos)->member), typeof(*(pos)), member)

#define rb_for_each_entry(pos, root, member) \
    for (pos = rb_first_entry(root, typeof(*pos), member); \
         pos; pos = rb_next_entry(pos, member))

/* Postorder iteration (Depth-first) - always visit the parent after its children */
extern struct rb_node *rb_post_first(const struct rb_root *root);
extern struct rb_node *rb_post_next(const struct rb_node *node);

#define rb_post_first_entry(ptr, type, member) \
    rb_entry_safe(rb_post_first(ptr), type, member)

#define rb_post_next_entry(pos, member) \
    rb_entry_safe(rb_post_next(&(pos)->member), typeof(*(pos)), member)

#define rb_post_for_each_entry(pos, root, member) \
    for (pos = rb_post_first_entry(root, typeof(*pos), member); \
         pos; pos = rb_post_next_entry(pos, member))

#define rb_post_for_each_entry_safe(pos, next, root, member) \
    for (pos = rb_post_first_entry(root, typeof(*pos), member); \
         pos && ({ next = rb_post_next_entry(pos, member); \
         1; }); pos = next)

/**
 * rb_link - link node to parent.
 * @parent: point to parent node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void rb_link(struct rb_node *parent,
                           struct rb_node **link, struct rb_node *node)
{
    /* link = &parent->left/right */
    *link = node;
    node->parent = parent;
    node->color = RB_RED;
    node->left = node->right = NULL;
}

/**
 * rb_insert_node - link node to parent and fixup rbtree.
 * @root: rbtree root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 */
static inline void rb_insert_node(struct rb_root *root, struct rb_node *parent,
                                  struct rb_node **link, struct rb_node *node)
{
    rb_link(parent, link, node);
    rb_fixup(root, node);
}

/**
 * rb_insert - find the parent node and insert new node.
 * @root: rbtree root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline int rb_insert(struct rb_root *root, struct rb_node *node,
                            long (*cmp)(const struct rb_node *, const struct rb_node *))
{
    struct rb_node *parent, **link;

    link = rb_parent(root, &parent, node, cmp, NULL);
    if (unlikely(!link))
        return -EINVAL;

    rb_insert_node(root, parent, link, node);
    return 0;
}

/**
 * rb_delete - delete node and fixup rbtree.
 * @root: rbtree root of node.
 * @node: node to delete.
 */
static inline void rb_delete(struct rb_root *root, struct rb_node *node)
{
    if ((node = rb_remove(root, node)))
        rb_erase(root, node);
}

/* Same as rb_first(), but O(1) */
#define rb_cached_first(cached) (cached)->rb_leftmost

#define rb_cached_first_entry(ptr, type, member) \
    rb_entry_safe(rb_cached_first(ptr), type, member)

#define rb_cached_for_each_entry(pos, cached, member) \
    for (pos = rb_cached_first_entry(cached, typeof(*pos), member); \
        pos; pos = rb_next_entry(pos, member))

#define rb_cached_post_for_each_entry(pos, cached, member) \
    rb_post_for_each_entry(pos, &(cached)->root, member)

#define rb_cached_post_for_each_entry_safe(pos, next, cached, member) \
    rb_post_for_each_entry_safe(pos, next, &(cached)->root, member)

/**
 * rb_cached_fixup - balance after insert cached node.
 * @cached: rbtree cached root of node.
 * @node: new inserted node.
 * @leftmost:
 */
static inline void rb_cached_fixup(struct rb_root_cached *cached,
                                   struct rb_node *node, bool leftmost)
{
    if (leftmost)
        cached->rb_leftmost = node;

    rb_fixup(&cached->root, node);
}

/**
 * rb_cached_insert_node - link cached node to parent and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @parent: parent node of node.
 * @link: point to pointer to child node.
 * @node: new node to link.
 * @leftmost: new node to link.
 */
static inline void rb_cached_insert_node(struct rb_root_cached *cached, struct rb_node *parent,
                                         struct rb_node **link, struct rb_node *node, bool leftmost)
{
    rb_link(parent, link, node);
    rb_cached_fixup(cached, node, leftmost);
}

/**
 * rb_cached_insert - find the parent node and insert new cached node.
 * @cached: rbtree cached root of node.
 * @node: new node to insert.
 * @cmp: operator defining the node order.
 */
static inline int rb_cached_insert(struct rb_root_cached *cached, struct rb_node *node,
                                   long (*cmp)(const struct rb_node *, const struct rb_node *))
{
    struct rb_node *parent, **link;
    bool leftmost = true;

    link = rb_parent(&cached->root, &parent, node, cmp, &leftmost);
    if (unlikely(!link))
        return -EINVAL;

    rb_cached_insert_node(cached, parent, link, node, leftmost);
    return 0;
}

/**
 * rb_cached_delete - delete cached node and fixup rbtree.
 * @cached: rbtree cached root of node.
 * @node: node to delete.
 */
static inline struct rb_node *rb_cached_delete(struct rb_root_cached *cached, struct rb_node *node)
{
    struct rb_node *leftmost = NULL;

    if (cached->rb_leftmost == node)
        leftmost = cached->rb_leftmost = rb_next(node);

    rb_delete(&cached->root, node);
    return leftmost;
}

/**
 * rb_cached_replace - replace old cached node by new cached one.
 * @root: rbtree root of node.
 * @old: node to be replaced.
 * @new: new node to insert.
 */
static inline void rb_cached_replace(struct rb_root_cached *cached, struct rb_node *old, struct rb_node *new)
{
    if (cached->rb_leftmost == old)
        cached->rb_leftmost = new;

    rb_replace(&cached->root, old, new);
}

#endif  /* _RBTREE_H_ */
