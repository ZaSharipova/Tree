#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <stdio.h>

#include "Enums.h"

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, char *name, char *question, size_t *size);
TreeErrors Akinator(Tree_t *tree, TreeNode_t *node, DumpInfo *Info);

void PrintAkinatorToFile(FILE *file, TreeNode_t *node);

TreeErrors DoPrintDefinition(TreeNode_t *node, const char *value, size_t count);
TreeErrors PrintDefinition(TreeNode_t *current, TreeNode_t *prev, char **array_of_definitions, size_t *pos);
TreeErrors SayDefinition(char **array_of_definitions, size_t count);

TreeErrors CompareResults(TreeNode_t *node, const char *value1, const char *value2, int count);


TreeErrors DoBufRead(FILE *file, const char *filename, FileInfo *Info);
TreeNode_t *ReadNodeFromFile(FILE *file, size_t *pos, TreeNode_t *node, TreeElem_t buffer, int *error);
//TreeErrors ReadAkinatorTreeFromFilename(const char *filename, TreeNode_t **root);
#endif //AKINATOR_H_