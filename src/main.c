#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "record.h"
#include "quick_sort_records.h"
#include "getch.h"
#include "shannon.h"

char *db_file;

void handle_args(int argc, char **argv) {
    if (argc < 2) {
        printf("Please, specify a database file.\n");
        exit(0);
    }
    db_file = (char *) malloc(strlen(argv[1]) * sizeof(char));
    strcpy(db_file, argv[1]);
    if (access(db_file, F_OK | R_OK) == -1) {
        printf("Cannot read the file '%s'\n", db_file);
        exit(0);
    }
}

int menu() {
    char in;

    FILE *file = fopen(db_file, "rb");
    struct record **rec_arr = read_records_to_array(file, DB_SIZE);
    fclose(file);
    file = fopen(db_file, "rb");
    struct record **s_rec_arr = read_records_to_array(file, DB_SIZE);
    quick_sort_records(s_rec_arr, 0, DB_SIZE - 1);
    fclose(file);
    struct queue *found = NULL;
    struct avl_node *avl_root = NULL;

    while (1) {
        system("clear");
        printf("1. Print database\n"
               "2. Print sorted\n"
               "3. Search database\n"
               "4. Create AVL tree (sorted by name)\n"
               "5. Print AVL tree\n"
               "6. Search in AVL tree\n"
               "7. Shannon code\n"
               "0. Exit\n");
        in = getch();
        switch (in) {
            case '1':
                print_record_array_partially(rec_arr, DB_SIZE, 10);
                break;
            case '2':
                print_record_array_partially(s_rec_arr, DB_SIZE, 10);
                break;
            case '3': {
                    char *search_key = (char *) malloc(44 * sizeof(char *));
                    printf("Enter your search key (at least 3 characters): ");
                    do {
                        fgets(search_key, 44, stdin);
                    } while (strlen(search_key) < 3);
                    found = find_records(s_rec_arr, DB_SIZE, search_key);
                    print_record_queue_partially(found, 10);
                    free(search_key);
                }
                break;
            case '4':
                if (found == NULL) {
                    printf("Queue is empty.\n"
                           "To fill it, run DB search.\n");
                } else {
                    avl_root = create_record_tree_from_queue(found);
                    printf("Tree created.\n");
                }
                break;
            case '5':
                if (avl_root != NULL) print_record_tree(avl_root);
                else printf("Tree is not created!\n");
                break;
            case '6':
                if (avl_root != NULL) {
                    char *search_key = (char *) malloc(64 * sizeof(char *));
                    printf("Enter your search key (<= 30 chars): ");
                    fgets(search_key, 64, stdin);
                    print_table_separator();
                    print_record(1, find_record_in_tree(avl_root, search_key));
                    print_table_separator();
                    free(search_key);
                } else {
                    printf("Tree is not created!\n");
                }
                break;
            case '7':
                file = fopen(db_file, "rb");
                count_codes(file);
                fclose(file);
                file = fopen(db_file, "rb");
                write_compressed(file);
                fclose(file);
                break;
            case '0':
                return 0;
            default:
                break;
        }
        printf("Press any key to continue...\n");
        getch();
    }
}

int main(int argc, char **argv) {
    handle_args(argc, argv);
    return menu();
}