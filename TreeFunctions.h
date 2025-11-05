#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H_

#include <stdio.h>

#include "Enums.h"

TreeErrors NodeCtor(TreeNode_t *node, TreeElem_t value);
TreeErrors NodeDtor(TreeNode_t *node);
void PrintNode(const TreeNode_t *node);
void PrintSortedNode(const TreeNode_t *node);

#endif //TREE_FUNCTIONS_H_