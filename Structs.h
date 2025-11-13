#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>

#include "Enums.h"

struct TreeNode_t {
    TreeElem_t data;
    TreeNode_t *parent;
    TreeNode_t *left;
    TreeNode_t *right;
};

struct Tree_t {
    TreeNode_t *root;
    size_t size;
};

typedef struct DumpInfo {
    Tree_t *tree;
    const char *filename_to_write_dump;
    FILE *file;
    const char *filename_to_write_graphviz;
    const char *filename_dump_made;
    char message[MAX_IMAGE_SIZE];
    char *name;
    char *question;
    char image_file[MAX_IMAGE_SIZE];
    size_t graph_counter;
    bool flag_new;

    enum TreeErrors error;
} DumpInfo;

struct FileInfo {
    TreeElem_t buf_ptr;
    size_t filesize;
};

struct Stack_Info {
    TreeNode_t **data;
    ssize_t size;
    ssize_t capacity;
};

#define RESET   "\033[0m"
#define WHITE   "\033[1;30m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"

#endif //STRUCTS_H_