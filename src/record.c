#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "getch.h"
#include "record_utils.h"
#include "avl_tree.h"

#define TABLE_HEADER    "|     # | Name                          | Sum        | Date      | Attorney              |\n"
#define TABLE_SEPARATOR "|----------------------------------------------------------------------------------------|\n"

#define NEW(type) (type *) malloc(sizeof(type))
#define NEW_N(type, amount) (type *) malloc(sizeof(type) * amount)

//TODO: FIX MEMORY LEAKS

void print_table_header() {
    printf(TABLE_SEPARATOR);
    printf(TABLE_HEADER);
    printf(TABLE_SEPARATOR);
}

void print_table_separator() {
    printf(TABLE_SEPARATOR);
}

struct record *read_record(FILE *file) {
    struct record *rec = NEW(struct record);
    fread(rec, sizeof(struct record), 1, file);
    return rec;
}

struct queue *add_record_to_queue(struct queue *q, struct record *rec) {
    struct list_entry *le = NEW(struct list_entry);
    le->data = rec;
    le->next = NULL;
    if (q == NULL) {
        q = NEW(struct queue);
        q->head = le;
        q->tail = le;
        return q;
    } else {
        q->tail->next = le;
        q->tail = le;
        return q;
    }
}

struct queue *read_records_to_queue(FILE *file) {
    struct queue *rec_queue = NEW(struct queue);
    struct list_entry *le = NEW(struct list_entry);
    rec_queue->head = le;
    rec_queue->tail = NULL;
    for (int i = 0; i < DB_SIZE; i++) {
        le->data = read_record(file);
        if (i < DB_SIZE - 1) {
            le->next = NEW(struct list_entry);
            le = le->next;
        }
    }
    rec_queue->tail = le;
}

struct record **read_records_to_array(FILE *file, int n) {
    struct record **rec_arr = NEW_N(struct record *, n);
    for (int i = 0; i < n; i++) {
        rec_arr[i] = read_record(file);
    }
    return rec_arr;
}

void print_record(int n, struct record *rec) {
    if (rec) {
        printf("| %5d | %s | %5d      | %s | %s |",
               n, rec->name, rec->sum, rec->date, rec->attorney);
    }
}

void print_record_queue(struct queue *rec_queue) {
    if (rec_queue == NULL) {
        printf("Queue is empty!\n");
        return;
    }
    struct list_entry *le = rec_queue->head;
    int i = 1;
    print_table_header();
    while (le != NULL) {
        print_record(i++, le->data);
        print_table_separator();
        le = le->next;
    }
}

void print_record_queue_partially(struct queue *rec_queue, int part) {
    if (rec_queue == NULL) {
        printf("Queue is empty!\n");
        return;
    }
    system("clear");
    struct list_entry *le = rec_queue->head;
    char in;
    int i = 1;
    print_table_header();
    while (le != NULL) {
        print_record(i++, le->data);
        print_table_separator();
        le = le->next;
        if ((i - 1) % part == 0 && i - 1 != 0) {
            printf("Enter - continue\nQ - quit\n");
            in = 0;
            while (in != 'Q' && in != 'q' && in != '\n' && in != ' ') in = getch();
            if (in == 'q' || in == 'Q') return;
            system("clear");
            print_table_header();
        }
    }
}

void print_record_array(struct record **rec_array, int n) {
    print_table_header();
    for (int i = 0; i < n; i++) {
        print_record(i + 1, rec_array[i]);
        print_table_separator();
    }
}

void print_record_array_partially(struct record **rec_array, int n, int part) {
    char in;
    system("clear");
    print_table_header();
    for (int i = 0; i < n; i++) {
        print_record(i + 1, rec_array[i]);
        print_table_separator();
        if ((i + 1) % part == 0) {
            printf("R - Forward\nE - Back\nQ - Quit\n");
            in = 0;
            while (in != 'Q' && in != 'q' && in != 'r' && in != 'R' && in != 'e' && in != 'E') in = getch();
            if (in == 'q' || in == 'Q') return;
            else if (in == 'e' || in == 'E') {
                i -= part * 2;
                if (i < -1) i = -1;
            }
            system("clear");
            print_table_header();
        }
    }
}

int print_record_tree_nodes(struct avl_node *node, int i, int n) {
    if (node == NULL) return 0;
    if (node->left != NULL) i = print_record_tree_nodes(node->left, i, n);
    print_record(i++, node->rec);
    print_table_separator();
    if (node->right != NULL) i = print_record_tree_nodes(node->right, i, n);
    return i;
}

void print_record_tree(struct avl_node *root) {
    system("clear");
    print_table_header();
    print_record_tree_nodes(root, 1, avl_size(root));
}

struct avl_node *create_record_tree_from_queue(struct queue *q) {
    struct list_entry *le = q->head;
    struct avl_node *root = NULL;
    while (le != NULL) {
        root = avl_insert(root, le->data);
        le = le->next;
    }
    return root;
}

struct queue *find_records(struct record **rec_arr, int n, char *key) {
    int i = binary_search_by_attorney(rec_arr, n, key);
    if (i == -1) return NULL;
    struct queue *res = NULL;
    for (; i < n && strncmp(rec_arr[i]->attorney, key, 3) == 0; i++) {
        res = add_record_to_queue(res, rec_arr[i]);
    }
    return res;
}

struct record *find_record_in_tree(struct avl_node *n, char *key) {
    if (n == NULL) return NULL;
    if (compare_strings_n(key, n->rec->name, strlen(key) - 1) == 0) return n->rec;
    else if (compare_strings(key, n->rec->name) < 0) return find_record_in_tree(n->left, key);
    else return find_record_in_tree(n->right, key);
}

void print_attorneys(struct record **rec_arr, int n) {
    char *att = NEW_N(char, 30);
    char *tmp = NEW_N(char, 64);
    for (int i = 0; i < n; i++) {
        if (strcmp(att, rec_arr[i]->attorney) != 0) {
            strcpy(att, rec_arr[i]->attorney);
            printf("%s\n", att);
        }
    }
    free(tmp);
    free(att);
}
