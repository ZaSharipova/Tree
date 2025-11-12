#ifndef READ_TREE_H_
#define READ_TREE_H_

#include <stdio.h>
#include "Enums.h"

TreeNode_t *ReadNodeFromFile(FILE *file, FILE *logfile, size_t *pos, TreeNode_t *node, TreeElem_t buffer, int *error);

#endif //READ_TREE_H_