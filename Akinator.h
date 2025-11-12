#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <stdio.h>

#include "Enums.h"

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, char *name, char *question, Tree_t *tree);
TreeErrors Akinator(Tree_t *tree, TreeNode_t *node, DumpInfo *Info);

void PrintAkinatorToFile(FILE *file, TreeNode_t *node);

TreeErrors DoPrintDefinition(TreeNode_t *node, const char *value, size_t tree_size, size_t count);
TreeErrors PrintDefinition(TreeNode_t *current, TreeNode_t *prev, char *definition_str, size_t buffer_len, int *pos_in_phrases);
TreeErrors SayDefinition(char **array_of_definitions, size_t count);

TreeErrors CompareNames(TreeNode_t *head, const char *value1, const char *value2);


TreeErrors DoBufRead(FILE *file, const char *filename, FileInfo *Info);

//TreeErrors ReadAkinatorTreeFromFilename(const char *filename, TreeNode_t **root);
#endif //AKINATOR_H_