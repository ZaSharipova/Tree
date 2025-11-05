#ifndef TREE_GRAPH_H_
#define TREE_GRAPH_H_

#include <stdio.h>

#include "Enums.h"

void PrintDotNode(FILE *file, const TreeNode_t *node);
void WriteTreeToDotFile(const Tree_t *tree, const char *filename);

#endif //TREE_GRAPH_H_