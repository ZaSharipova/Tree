#ifndef TREE_GRAPH_H_
#define TREE_GRAPH_H_

#include <stdio.h>

#include "Enums.h"
#include "Structs.h"

void PrintDotNode(FILE *file, const TreeNode_t *node, const TreeNode_t *node_colored);
void DoTreeInGraphviz(const TreeNode_t *tree, DumpInfo *Info, const TreeNode_t *node_colored);

#endif //TREE_GRAPH_H_