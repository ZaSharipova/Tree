#ifndef TREE_FUNCTIONS_H_
#define TREE_FUNCTIONS_H_

#include <stdio.h>

#include "Enums.h"

#define MAX_ARR_SIZE 23

#define INSERT_TO_TREE(value) \
    number = value; \
    InsertTree(&tree, &number);

TreeErrors TreeCtor(Tree_t *tree);
TreeErrors NodeCtor(TreeNode_t **node, TreeElem_t *value);

TreeErrors TreeDtor(Tree_t *tree);
TreeErrors NodeDtor(TreeNode_t *node);

TreeErrors InsertTree(Tree_t *tree, TreeElem_t *value);
TreeErrors InsertNode(TreeNode_t *parent_node, TreeElem_t *value);
void PrintNode(const TreeNode_t *node);
void PrintSortedNode(const TreeNode_t *node, TreeElem_t *arr_after);

TreeErrors DeleteNode(TreeNode_t *node);

bool CompareNodes(TreeElem_t parent_value, TreeElem_t children_value);
#endif //TREE_FUNCTIONS_H_