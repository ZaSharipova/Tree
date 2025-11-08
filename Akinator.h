#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <stdio.h>

#include "Enums.h"

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, const char *name, char *question);
TreeErrors Akinator(TreeNode_t *node);

void PrintAkinatorToFile(FILE *file, TreeNode_t *node);
TreeErrors PrintDefinition(TreeNode_t *node, const char *value, int count);
#endif //AKINATOR_H_