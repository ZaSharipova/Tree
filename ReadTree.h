#ifndef READ_TREE_H_
#define READ_TREE_H_

#include <stdio.h>

#include "Enums.h"
#include "Structs.h"

TreeErrors ReadNodeFromFile(Tree_t *tree, FILE *file, FILE *logfile, size_t *pos, TreeNode_t *node, TreeElem_t buffer, TreeNode_t **node_to_add);

#endif //READ_TREE_H_