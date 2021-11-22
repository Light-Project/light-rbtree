/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include "rbtree.h"
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#define TEST_LEN 100

struct rbtree_demo {
    struct rb_node rb;
    unsigned int num;
    unsigned long data;
};

#define demo_rb_entry(node) \
    container_of(node, struct rbtree_demo, rb)

static RB_ROOT(demo_root);

static inline struct rb_node **
node_parent(struct rbtree_demo *new, struct rb_root *root, struct rb_node **parentp)
{
    struct rbtree_demo *parent;
    struct rb_node **link;

    link = &root->rb_node;

    if (unlikely(!*link)) {
        *parentp = NULL;
        return link;
    }

    do {
        parent = demo_rb_entry(*link);

        if (new->data < parent->data)
            link = &(*link)->left;
        else if (new->data > parent->data)
            link = &(*link)->right;
        else
            return NULL;

    } while (*link);

    *parentp = &parent->rb;
    return link;
}

static inline state node_insert(struct rbtree_demo *node)
{
    struct rb_node *parent, **link;

    link = node_parent(node, &demo_root, &parent);
    if (unlikely(!link))
        return -EFAULT;

    rb_link(parent, link, &node->rb);
    rb_fixup(&demo_root, &node->rb);

    return 0;
}

static inline void node_dump(struct rbtree_demo *node)
{
    printf("  %08d: data 0x%08lx node %16p parent %16p left %16p right %16p color '%s'\n",
                node->num, node->data, &node->rb, node->rb.parent,
                node->rb.left, node->rb.right, node->rb.color ? "black" : "red");
}

int main(void)
{
    struct rbtree_demo *node;
    unsigned int count;

    printf("Generate node:\n");

    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if (!node)
            err(1, "insufficient memory\n");

        node->num = count;
        node->data = ((unsigned long)rand() << 32) | rand();
        printf("  %08d: 0x%08lx\n", count, node->data);

        if (node_insert(node))
            err(1, "area overlaps\n");
    }

    printf("Middle iteration:\n");
    rb_for_each_entry(node, &demo_root, rb)
        node_dump(node);

    printf("Postorder iteration:\n");
    rb_post_for_each_entry(node, &demo_root, rb)
        node_dump(node);

    return 0;
}
