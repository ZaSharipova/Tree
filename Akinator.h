#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <stdio.h>

#include "Enums.h"
#include "Structs.h"

#define MAX_PHRASE_SIZE 130
#define PRINT_AND_SYSTEM(format, ...) do {                                     \
    printf(format, ##__VA_ARGS__);                                             \
    fflush(stdout);                                                            \
    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(cmd, MAX_PHRASE_SIZE * 4);              \
    snprintf(cmd, MAX_PHRASE_SIZE * 4, "say \"" format "\" &", ##__VA_ARGS__); \
    system(cmd);                                                               \
    free(cmd);                                                                 \
} while(0)

#define DO_CALLOC_AND_CHECK_PROBLEM_RETURN(ptr, size)                   \
    char *ptr = (char *) calloc (size, sizeof(char));                   \
    if (!ptr) {                                                         \
        fprintf(stderr, "No memory in calloc.\n");                      \
        return kNoMemory;                                               \
    }

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, char *name, char *question, Tree_t *tree);
TreeErrors Akinator(Tree_t *tree, TreeNode_t *node, DumpInfo *Info, TypeOfAnswer *play_again_flag);

void PrintAkinatorToFile(FILE *file, TreeNode_t *node);

TreeErrors DoPrintDefinition(TreeNode_t *node, const char *value, size_t tree_size, size_t count);
TreeErrors PrintDefinition(TreeNode_t *current, TreeNode_t *prev, char *definition_str);
TreeErrors SayDefinition(char **array_of_definitions, size_t count);

TreeErrors CompareNames(TreeNode_t *head, const char *value1, const char *value2, size_t pos);


TreeErrors DoBufRead(FILE *file, const char *filename, FileInfo *Info);
TreeErrors AskAndDoFileRead(Tree_t *tree, DumpInfo *Info, FileInfo *FileInfo, FILE *file_in);
TreeErrors DoDifferentAkinatorModes(Tree_t *tree, DumpInfo *Info, size_t pos);

TreeErrors AskAndPrintAkinatorToFile(FILE *file_out, TreeNode_t *head);
//TreeErrors ReadAkinatorTreeFromFilename(const char *filename, TreeNode_t **root);
#endif //AKINATOR_H_