#ifndef DSA_COURSEWORK_RECORD_H
#define DSA_COURSEWORK_RECORD_H

#include <stdio.h>

#define DB_SIZE 4000

struct record {
    char name[30];
    unsigned short int sum;
    char date[10];
    char attorney[22];
};

struct avl_node {
    struct record *rec;
    struct avl_node *left;
    struct avl_node *right;
    int height;
};

struct list_entry {
    struct record *data;
    struct list_entry *next;
};

struct queue {
    struct list_entry *head;
    struct list_entry *tail;
};

void print_table_header();

void print_table_separator();

struct record *read_record(FILE *file);

struct queue *read_records_to_queue(FILE *file);

struct record **read_records_to_array(FILE *file, int n);

void print_record(int n, struct record *rec);

void print_record_queue(struct queue *rec_queue);

void print_record_queue_partially(struct queue *rec_queue, int part);

void print_record_array(struct record **rec_array, int n);

void print_record_array_partially(struct record **rec_array, int n, int part);

void print_record_tree(struct avl_node *root);

struct avl_node *create_record_tree_from_queue(struct queue *q);

struct queue *find_records(struct record **rec_arr, int n, char *key);

struct record *find_record_in_tree(struct avl_node *n, char *key);

void print_attorneys(struct record **rec_arr, int n);

#endif //DSA_COURCEWORK_RECORD_H
