#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"
#include "record_utils.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(struct avl_node *n) {
    if (n == NULL) return 0;
    return n->height;
}

int avl_size(struct avl_node *n) {
    if (n == NULL) return 0;
    else return avl_size(n->left) + avl_size(n->right) + 1;
}

struct avl_node *new_node(struct record *rec) {
    struct avl_node *n = (struct avl_node *) malloc(sizeof(struct avl_node));
    n->rec = rec;
    n->left = NULL;
    n->right = NULL;
    n->height = 1;
    return n;
}

struct avl_node *right_rotate(struct avl_node *y) {
    struct avl_node *x = y->left;
    struct avl_node *t2 = x->right;
    x->right = y;
    y->left = t2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

struct avl_node *left_rotate(struct avl_node *x) {
    struct avl_node *y = x->right;
    struct avl_node *t2 = y->left;
    y->left = x;
    x->right = t2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int get_balance(struct avl_node *n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

struct avl_node *avl_insert(struct avl_node *n, struct record *rec) {
    if (n == NULL) return new_node(rec);
    if (compare_records_by_name(rec, n->rec) < 0) n->left = avl_insert(n->left, rec);
    else if (compare_records_by_name(rec, n->rec) > 0) n->right = avl_insert(n->right, rec);
    else return n;
    n->height = 1 + max(height(n->left), height(n->right));
    int balance = get_balance(n);
    if (balance > 1 && compare_records_by_name(rec, n->left->rec) < 0) return right_rotate(n);
    if (balance < -1 && compare_records_by_name(rec, n->right->rec) > 0) return left_rotate(n);
    if (balance > 1 && compare_records_by_name(rec, n->left->rec) > 0) {
        n->left = left_rotate(n->left);
        return right_rotate(n);
    }
    if (balance < -1 && compare_records_by_name(rec, n->right->rec) < 0) {
        n->right = right_rotate(n->right);
        return left_rotate(n);
    }
    return n;
}

