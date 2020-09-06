#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btree.h"


int main() {
    b_tree_node *head = init_b_tree_head();


    b_tree_add(head->head, 6 , 0, NULL);

    b_tree_add(head->head, 2, 0, NULL);
    b_tree_add(head->head, 4, 0, NULL);
    b_tree_add(head->head, 1, 0, NULL);
    b_tree_add(head->head, 5, 0, NULL);
    b_tree_add(head->head, 7, 0, NULL);
    b_tree_add(head->head, 8, 0, NULL);

    for (unsigned i = 0; i < 11; i++)
        b_tree_add(head->head, i + 1, 0, NULL);


    print_tree(head->head);


    return 0;
}
