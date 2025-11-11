#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <stdio.h>

#include "Enums.h"

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, char *name, char *question);
TreeErrors Akinator(TreeNode_t *head, TreeNode_t *node, DumpInfo *Info);

void PrintAkinatorToFile(FILE *file, TreeNode_t *node);
TreeErrors PrintDefinition(TreeNode_t *node, const char *value, size_t count);
TreeErrors CompareResults(TreeNode_t *node, const char *value1, const char *value2, int count);

//TreeErrors ReadAkinatorTreeFromFilename(const char *filename, TreeNode_t **root);
#endif //AKINATOR_H_