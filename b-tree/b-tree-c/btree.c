//
// Created by Key Real
//

#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

b_tree_node *init_b_tree_head() {

    b_tree_node *node = (b_tree_node *) calloc(1, sizeof(b_tree_node));

    if (node == NULL) {
        printf("Error: cant allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        node->data_[i].value_ = 0;
        node->data_[i].key_ = 0;
        node->ptrs_[i] = NULL;
    }
    node->ptrs_[N] = NULL;
    node->cur_N = 0;
    node->head = node;

    return node;
}


void b_tree_add(b_tree_node *node, uint64_t key, uint64_t value, b_tree_node *ptr) {
    if(!node) return;

    if(node->cur_N == 0) {
        printf("I'm %p, adding %llu, new ptr %p\n", node, key, ptr);
        node->data_[0].value_ = value;
        node->data_[0].key_ = key;
        node->cur_N++;
        return;
    }

    for(unsigned i = 0 ; i < N; i++) {
        uint64_t N_key = node->data_[i].key_;
        if(key == N_key) {
            node->data_[i].value_ = value;
            return;
        }
        if(key < N_key) {
            printf("Key %llu < N<key, i'm in %p, i = %i\n", key, node, i);
            if(node->ptrs_[i] == NULL) {
                //getchar();
                if(node->cur_N == N) //full
                    node_split(node, key, value);
                else {
                    add_by_pos(node, i, key, value, ptr);
                }
            }
            else {
                printf("Going to ptr %p\n", node->ptrs_[i]);
                b_tree_add(node->ptrs_[i], key, value, ptr);
                node->ptrs_[i]->parent = node;
            }
            return;
        }
    }
    if (key > node->data_[node->cur_N - 1].key_) {
        printf("Key %llu >... cur_N = %i\n", key, node->cur_N);
        if(node->ptrs_[node->cur_N] == NULL) {
            if(node->cur_N == N) //full
                node_split(node, key, value);

            else {
                printf("I'm Greater, node %p, adding %llu, new ptr %p\n", node, key, ptr);
                node->data_[node->cur_N].key_ = key;
                node->data_[node->cur_N].value_ = value;
                node->cur_N++;
            }
        }
        else {
            b_tree_add(node->ptrs_[node->cur_N], key, value, ptr);
        }
    }
}


void add_by_pos(b_tree_node *node, uint64_t pos, uint64_t key, uint64_t value, b_tree_node *ptr_right) {
    if(!node) return;
    printf("I'm shifter, node %p, adding %llu, new ptr %p\n", node, key, ptr_right);
    for(int i = node->cur_N; i > pos; i--) {
        node->data_[i].value_ = node->data_[i-1].value_;
        node->data_[i].key_ = node->data_[i-1].key_;
        node->ptrs_[i+1] = node->ptrs_[i];
    }
    node->ptrs_[pos] = ptr_right;
    node->data_[pos].value_ = value;
    node->data_[pos].key_ = key;
    node->cur_N++;
}


void node_split(b_tree_node *node, uint64_t key, uint64_t value) {
    printf("I'm splitter, key %llu\n", key);
    b_data middle = node->data_[N / 2];
    b_tree_node * right = init_b_tree_head();

    //copy right half to that
    right->ptrs_[0] = node->ptrs_[N / 2];
    for (int i = N / 2 + 1, j = 0; i < N; i++, j++) {
        printf("Copying %llu from %p to %p\n", node->data_[i].key_, node, right);
        right->data_[j].value_ = node->data_[i].value_;
        right->data_[j].key_ = node->data_[i].key_;
        right->cur_N++;
        right->ptrs_[j + 1] = node->ptrs_[i + 1];
    }

    if (node->parent != 0) {
        printf("Parent != 0\n");
        right->parent = node->parent;


        printf("Going to add in parent %llu\n", middle.key_);
        if(node->parent->cur_N == N) //full
            node_split(node->parent, middle.key_, middle.value_);
        else {
            node->parent->data_[node->parent->cur_N].key_ = middle.key_;
            node->parent->data_[node->parent->cur_N++].value_ = middle.value_;
            node->parent->ptrs_[node->parent->cur_N] = node;
            node->parent->ptrs_[node->parent->cur_N] = right;
        }

    }
    else {
        node->parent = init_b_tree_head();
        right->parent = node->parent;
        printf("Adding %llu to %p\n", middle.key_, node->parent);
        node->parent->data_[0].key_ = middle.key_;
        node->parent->data_[0].value_ = middle.value_;
        node->parent->cur_N++;
        node->parent->ptrs_[0] = node;
        node->parent->ptrs_[1] = right;

        node->parent->head = node->parent;
        node->head = node->parent;
    }



    //delete middle and right from left
    for (int i = N / 2; i < N; i++) {
        printf("Deleting %llu from %p\n", node->data_[i].key_, node);
        node->data_[i].value_ = 0;
        node->data_[i].key_ = 0;
        node->ptrs_[i + 1] = NULL;
        node->cur_N = N / 2;
    }

    b_tree_add(node->parent, key, value, NULL);
}


void delete_elem(b_tree_node *tree, const uint64_t key) {
    find_key(tree, key)->deleted_ = 1;
};

b_data * find_key(b_tree_node *node, const uint64_t key){
    if (!node)
        return NULL;

    for (int i = 0; i < N; i++) {
        if (node->data_[i].key_ == key)
            return &node->data_[i];
        if (node->data_[i].key_ > key){
            return find_key(node->ptrs_[i], key);
            //go left

        }
    };
    return find_key(node->ptrs_[N], key);
    //go right
};


FILE *super_tree_dump;

void print_tree(b_tree_node *tree) {
    if ((super_tree_dump = fopen("dump.gv", "w")) == NULL) {
        printf("Cannot open file.n");
        exit(EXIT_FAILURE);
    }
    fprintf(super_tree_dump, "digraph G{\n");
    print_node(tree);

    fprintf(super_tree_dump, "}");
    fclose(super_tree_dump);
    system("dot -Tpng dump.gv -o dump.png");
}

void print_node(b_tree_node *node) {

    print_node_info(node);

    for (int j = 0; j < N + 1; j++) {

        if (!node->ptrs_[j])
            continue;

        fprintf(super_tree_dump, "node%p:port%i -> node%p\n", node, j, node->ptrs_[j]);

        print_node_info(node->ptrs_[j]);
    }
}


void print_node_info(b_tree_node *node) {
    fprintf(super_tree_dump, "node%p [\nshape=plaintext label = < <table border='1' cellborder='1'>\n<tr>", node);
    fprintf(super_tree_dump, "<td port='port0'> </td>");
    for (int i = 0; i < N; i++) {
        fprintf(super_tree_dump, "<td>%llu<br/>%llu</td>", node->data_[i].key_, node->data_[i].value_);
        fprintf(super_tree_dump, "<td port='port%i'> </td>", i + 1);
    }
    fprintf(super_tree_dump, "\n</tr></table> >]\n");
}
