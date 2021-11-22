/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include "rbtree.h"
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#define TEST_LEN 100
static RB_ROOT(demo_root);

struct rbtree_demo {
    struct rb_node rb;
    unsigned int num;
    unsigned long data;
};

#define rb_to_demo(node) \
    rb_entry(node, struct rbtree_demo, rb)

static long demo_cmp(const struct rb_node *new, const struct rb_node *node)
{
    struct rbtree_demo *demo_new = rb_to_demo(new);
    struct rbtree_demo *demo_node = rb_to_demo(node);
    return demo_node->data - demo_new->data;
}

static void node_dump(struct rbtree_demo *node)
{
    printf("  %08d: data 0x%016lx node %16p parent %16p left %16p right %16p color '%s'\n",
            node->num, node->data, &node->rb, node->rb.parent,
            node->rb.left, node->rb.right, node->rb.color ? "black" : "red");
}

int main(void)
{
    struct rbtree_demo *node, *tmp;
    unsigned int count;

    printf("Generate node:\n");

    for (count = 0; count < TEST_LEN; ++count) {
        node = malloc(sizeof(*node));
        if (!node) {
            printf("insufficient memory\n");
            goto error;
        }

        node->num = count;
        node->data = ((unsigned long)rand() << 32) | rand();
        printf("  %08d: 0x%016lx\n", count, node->data);

        if (rb_insert(&demo_root, &node->rb, demo_cmp)) {
            printf("area overlaps\n");
            goto error;
        }
    }

    printf("Middle iteration:\n");
    rb_for_each_entry(node, &demo_root, rb)
        node_dump(node);

    printf("Postorder iteration:\n");
    rb_post_for_each_entry(node, &demo_root, rb)
        node_dump(node);

    return 0;

error:
    rb_post_for_each_entry_safe(node, tmp, &demo_root, rb)
        free(node);

    return 1;
}
