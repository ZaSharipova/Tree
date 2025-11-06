#ifndef TREE_FUNCTIONS_H_
#define TREE_FUNCTIONS_H_

#include <stdio.h>

#include "Enums.h"

TreeErrors TreeCtor(Tree_t *tree);
TreeErrors NodeCtor(TreeNode_t **node, TreeElem_t *value);

TreeErrors TreeDtor(Tree_t *tree);
TreeErrors NodeDtor(TreeNode_t *node);

TreeErrors InsertTree(Tree_t *tree, TreeElem_t *value);
TreeErrors InsertNode(TreeNode_t *parent_node, TreeElem_t value);
TreeErrors InsertNodeChar(TreeNode_t *parent_node, TreeElem_t *value);
void PrintNode(const TreeNode_t *node);
void PrintSortedNode(const TreeNode_t *node);

TreeErrors DeleteNode(TreeNode_t *node);

#endif //TREE_FUNCTIONS_H_