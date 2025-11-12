#ifndef TREE_FUNCTIONS_H_
#define TREE_FUNCTIONS_H_

#include <stdio.h>
#include <stdbool.h>

#include "Enums.h"

#define MAX_ARR_SIZE 23

#define INSERT_TO_TREE(val) \
    value = val; \
    InsertTree(&tree, &value);

#define CHECK_ERROR_RETURN(cond) \
    err = cond;                  \
    if (err != kSuccess) {       \
        return err;              \
    }

#define DO_OPEN_FILE_RETURN(file_name_after_open, filename, open_mode) \
    FILE * file_name_after_open = fopen(filename, open_mode);          \
    if (file_name_after_open == NULL) {                                \
        perror("fopen() failed.");                                     \
        return kErrorOpeningFile;                                      \
    }

TreeErrors TreeRootCtor(Tree_t *tree);
TreeErrors NodeCtor(TreeNode_t **node, TreeElem_t *value);

TreeErrors TreeDtor(Tree_t *tree);
TreeErrors NodeDtor(TreeNode_t *node);
TreeErrors TreeVerify(const TreeNode_t *head, int size, int *cnt);
TreeErrors NodeVerify(const TreeNode_t *node);

TreeErrors InsertTree(Tree_t *tree, TreeElem_t *value);
TreeErrors InsertNode(TreeNode_t *parent_node, TreeElem_t *value);

void PrintNodePreOrder(const TreeNode_t *node);
void PrintNodePostOrder(const TreeNode_t *node);
void PrintNodeInOrder(const TreeNode_t *node);

void PrintSortedNode(const TreeNode_t *node);
void SortNodeToArray(const TreeNode_t *node, TreeElem_t *arr_after, int *i);

TreeErrors DeleteNode(TreeNode_t *node);

bool CompareNodes(TreeElem_t parent_value, TreeElem_t children_value);

int CheckSorting(TreeElem_t *arr_to_check_with, TreeNode_t *head, TreeElem_t *arr_before, int count);

TreeNode_t *FindNodeAddress(TreeNode_t *head, TreeElem_t *value);

#endif //TREE_FUNCTIONS_H_