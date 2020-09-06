
//
// Created by Kil Real
//

#ifndef B_TREE_TREE_H
#define B_TREE_TREE_H


#include <stdint.h>
#define N 3


typedef struct b_data {
    uint64_t value_;
    uint64_t key_;
    int deleted_;
} b_data;

typedef struct b_tree_node {
    b_data data_[N];
    struct b_tree_node *ptrs_[N + 1];
    struct b_tree_node *parent;
    int cur_N;
    struct b_tree_node *head;
} b_tree_node;

/* init head of B-tree */
b_tree_node *init_b_tree_head();

void b_tree_add(b_tree_node *head, uint64_t key, uint64_t value, b_tree_node *ptr);
void add_by_pos(b_tree_node *node, uint64_t pos, uint64_t key, uint64_t value, b_tree_node *ptr_right);
void node_split();
void delete_elem(b_tree_node *tree, uint64_t key);
b_data * find_key(b_tree_node *tree, uint64_t key);

void print_tree(b_tree_node *tree);
void print_node(b_tree_node *node);
void print_node_info(b_tree_node *node);

#endif //B_TREE_TREE_H
