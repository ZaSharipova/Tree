#ifndef TREE_GRAPH_H_
#define TREE_GRAPH_H_

#include <stdio.h>

#include "Enums.h"
#include "Structs.h"

void PrintDotNode(FILE *file, const TreeNode_t *node);
void DoTreeInGraphviz(const TreeNode_t *tree, DumpInfo *Info);

#endif //TREE_GRAPH_H_