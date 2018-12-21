#ifndef DSA_COURSEWORK_AVL_TREE_H
#define DSA_COURSEWORK_AVL_TREE_H

#include "record.h"

int avl_size(struct avl_node *n);

struct avl_node *avl_insert(struct avl_node *n, struct record *rec);

#endif //DSA_COURSEWORK_AVL_TREE_H
