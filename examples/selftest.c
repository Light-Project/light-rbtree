/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * ARRAY_SIZE - get the number of elements in array.
 * @arr: array to be sized.
 */
#define ARRAY_SIZE(arr) ( \
    sizeof(arr) / sizeof((arr)[0]) \
)

#define TEST_LOOP 10

struct rbtree_test_node {
    struct rb_node node;
    unsigned long num;
};

struct rbtree_test_pdata {
    struct rbtree_test_node nodes[TEST_LOOP];
};

#define rbnode_to_test(ptr) \
    rb_entry(ptr, struct rbtree_test_node, node)

static long rbtest_rb_cmp(const struct rb_node *rba, const struct rb_node *rbb)
{
    struct rbtree_test_node *nodea = rbnode_to_test(rba);
    struct rbtree_test_node *nodeb = rbnode_to_test(rbb);
    return nodea->num - nodeb->num;
}

static int rbtree_test_testing(struct rbtree_test_pdata *sdata)
{
    struct rbtree_test_node *node, *nnode, *tnode;
    struct rb_node *rbnode, *nrbnode, *trbnode;
    unsigned long count;

    RB_ROOT_CACHED(test_root);

    for (count = 0; count < ARRAY_SIZE(sdata->nodes); ++count)
        rb_cached_insert(&test_root, &sdata->nodes[count].node, rbtest_rb_cmp);

    count = 0;
    rb_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    rb_for_each_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_continue' test: %lu\n", node->num);
    }

    rbnode = trbnode;
    rb_for_each_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_for_each_reverse(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_reverse' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    rb_for_each_reverse_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_reverse_continue' test: %lu\n", node->num);
    }

    rbnode = trbnode;
    rb_for_each_reverse_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_for_each_reverse_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_post_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    rb_post_for_each_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_continue' test: %lu\n", node->num);
    }

    rbnode = trbnode;
    rb_post_for_each_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_post_for_each_safe(rbnode, nrbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_safe' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    rb_post_for_each_safe_continue(rbnode, nrbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_safe_continue' test: %lu\n", node->num);
    }

    rbnode = trbnode;
    rb_post_for_each_safe_from(rbnode, nrbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each_safe_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_for_each_entry(node, &test_root.root, node) {
        printf("rbtree 'rb_for_each_entry' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    rb_for_each_entry_continue(node, node) {
        printf("rbtree 'rb_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    rb_for_each_entry_from(node, node) {
        printf("rbtree 'rb_for_each_entry_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_for_each_entry_reverse(node, &test_root.root, node) {
        printf("rbtree 'rb_for_each_entry_reverse' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    rb_for_each_entry_reverse_continue(node, node) {
        printf("rbtree 'rb_for_each_entry_reverse_continue' test: %lu\n", node->num);
    }

    node = tnode;
    rb_for_each_entry_reverse_from(node, node) {
        printf("rbtree 'rb_for_each_entry_reverse_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_post_for_each_entry(node, &test_root.root, node) {
        printf("rbtree 'rb_post_for_each_entry' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    count = 0;
    rb_pre_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_pre_for_each' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    rb_pre_for_each_continue(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_pre_for_each_continue' test: %lu\n", node->num);
    }

    rbnode = trbnode;
    rb_pre_for_each_from(rbnode) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_pre_for_each_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_pre_for_each_entry(node, &test_root.root, node) {
        printf("rbtree 'rb_pre_for_each_entry' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    rb_pre_for_each_entry_continue(node, node) {
        printf("rbtree 'rb_pre_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    rb_pre_for_each_entry_from(node, node) {
        printf("rbtree 'rb_pre_for_each_entry_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_level_for_each(rbnode, &count, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_level_for_each' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    trbnode = rbnode;
    rb_level_for_each_continue(rbnode, &count, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_level_for_each_continue' test: %lu\n", node->num);
    }

    rbnode = trbnode;
    rb_level_for_each_from(rbnode, &count, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_level_for_each_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_level_for_each_entry(node, &count, &test_root.root, node) {
        printf("rbtree 'rb_level_for_each_entry' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    rb_level_for_each_entry_from(node, &count, &test_root.root, node) {
        printf("rbtree 'rb_level_for_each_entry_from' test: %lu\n", node->num);
    }

    node = tnode;
    rb_level_for_each_entry_continue(node, &count, &test_root.root, node) {
        printf("rbtree 'rb_level_for_each_entry_continue' test: %lu\n", node->num);
    }

    count = 0;
    rb_post_for_each(rbnode, &test_root.root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_post_for_each' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    rb_post_for_each_entry_continue(node, node) {
        printf("rbtree 'rb_post_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    rb_post_for_each_entry_from(node, node) {
        printf("rbtree 'rb_post_for_each_entry_from' test: %lu\n", node->num);
    }

    count = 0;
    rb_post_for_each_entry_safe(node, nnode, &test_root.root, node) {
        printf("rbtree 'rb_post_for_each_entry_safe' test: %lu\n", node->num);
        if (count++ == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    rb_post_for_each_entry_continue_safe(node, nnode, node) {
        printf("rbtree 'rb_post_for_each_entry_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    rb_post_for_each_entry_from_safe(node, nnode, node) {
        printf("rbtree 'rb_post_for_each_entry_from_safe' test: %lu\n", node->num);
    }

    rb_cached_for_each(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_for_each' test: %lu\n", node->num);
    }

    rb_cached_for_each_reverse(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_for_each_reverse' test: %lu\n", node->num);
    }

    rb_cached_for_each_entry(node, &test_root, node) {
        printf("rbtree 'rb_cached_for_each_entry' test: %lu\n", node->num);
    }

    rb_cached_for_each_entry_reverse(node, &test_root, node) {
        printf("rbtree 'rb_cached_for_each_entry_reverse' test: %lu\n", node->num);
    }

    rb_cached_pre_for_each(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_pre_for_each' test: %lu\n", node->num);
    }

    rb_cached_pre_for_each_entry(node, &test_root, node) {
        printf("rbtree 'rb_cached_pre_for_each_entry' test: %lu\n", node->num);
    }

    rb_cached_level_for_each(rbnode, &count, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_level_for_each' test: %lu\n", node->num);
    }

    rb_cached_level_for_each_entry(node, &count, &test_root, node) {
        printf("rbtree 'rb_cached_level_for_each_entry' test: %lu\n", node->num);
    }

    rb_cached_post_for_each(rbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_post_for_each' test: %lu\n", node->num);
    }

    rb_cached_post_for_each_safe(rbnode, nrbnode, &test_root) {
        node = rbnode_to_test(rbnode);
        printf("rbtree 'rb_cached_post_for_each_safe' test: %lu\n", node->num);
    }

    rb_cached_post_for_each_entry(node, &test_root, node) {
        printf("rbtree 'rb_cached_post_for_each_entry' test: %lu\n", node->num);
    }

    rb_cached_post_for_each_entry_safe(node, nnode, &test_root, node) {
        printf("rbtree 'rb_cached_post_for_each_entry_safe' test: %lu\n", node->num);
        rb_cached_delete(&test_root, &node->node);
    }

    return 0;
}

int main(void)
{
    struct rbtree_test_pdata *rdata;
    unsigned int count;
    int retval;

    rdata = malloc(sizeof(struct rbtree_test_pdata));
    if (!rdata)
        return -1;

    for (count = 0; count < ARRAY_SIZE(rdata->nodes); ++count)
        rdata->nodes[count].num = TEST_LOOP - count - 1;

    retval = rbtree_test_testing(rdata);
    free(rdata);

    return retval;
}
