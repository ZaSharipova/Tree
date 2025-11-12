#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "TreeFunctions.h"
#include "Enums.h"
#include "TreeGraph.h"
#include "TreeDump.h"
#include "Akinator.h"

void printnode(TreeNode_t *node) {

    if (node) {
        printf("%s\n", node->data);
        printnode(node->left);
        printnode(node->right); 
    }
}
int main(void) {
    TreeErrors err = kSuccess;

    Tree_t tree = {};
    CHECK_ERROR_RETURN(TreeRootCtor(&tree));

    DumpInfo Info = {};
    Info.filename_to_write_dump = "alldump.html";
    Info.filename_to_write_graphviz = FILE_OUT;
    Info.file = fopen(Info.filename_to_write_dump, "w");
    Info.tree = &tree;

    FILE *file = fopen("akinator_in.txt", "r");
    if (file == NULL) {
        perror("fopen() failed.");
        return kErrorOpeningFile;
    }
    FileInfo FileInfo = {};
    DoBufRead(file, "akinator_in.txt", &FileInfo);
    size_t pos = 0;
    int error = 0;
    tree.root = ReadNodeFromFile(file, &pos, tree.root, FileInfo.buf_ptr, &error);
    if (error) {
        fprintf(stderr, "Ошибка при парсинге дерева\n");
        return 1; // завершение программы или обработка ошибки
    }
    fclose(file);

    // char *value = "No One knows";
    // INSERT_TO_TREE(value);
    printf("\n\n");

    //printnode(tree.root);
    DoTreeInGraphviz((const TreeNode_t *)tree.root, &Info);
    DoDump(&Info);
    CHECK_ERROR_RETURN(Akinator(&tree, tree.root, &Info));

    int cnt = 0;
    CHECK_ERROR_RETURN(TreeVerify(tree.root, (int)tree.size, &cnt));

    DoTreeInGraphviz(tree.root, &Info);

    // printf("%d ", tree.size);
    CHECK_ERROR_RETURN(DoPrintDefinition(tree.root, "Данис и Диаз", 1000));
    //CHECK_ERROR_RETURN(CompareResults(tree.root, "Зарина", "Исами", tree.size));

    // const char *number = "12";
    // INSERT_TO_TREE("10");
    // INSERT_TO_TREE("5");
    // INSERT_TO_TREE("3");
    // INSERT_TO_TREE("20");
    // INSERT_TO_TREE("7");
    // INSERT_TO_TREE("15");
    // INSERT_TO_TREE("13");
    // INSERT_TO_TREE("16");
    // INSERT_TO_TREE("6");

    // int value = 0;
    // INSERT_TO_TREE(10);
    // INSERT_TO_TREE(5);
    // INSERT_TO_TREE(3);
    // INSERT_TO_TREE(7);
    // INSERT_TO_TREE(20);
    // INSERT_TO_TREE(15);
    // INSERT_TO_TREE(13);
    // INSERT_TO_TREE(16);
    // INSERT_TO_TREE(6);

    // TreeElem_t arr_before[] = {10, 5, 3, 7, 20, 15, 13, 16, 6};
    // int count = sizeof(arr_before) / sizeof(arr_before[0]);
    // TreeElem_t arr_after[MAX_ARR_SIZE] = {10, 5, 3, 7, 20, 15, 13, 16, 6};

    // CHECK_ERROR_RETURN(TreeVerify(tree.root, (int)tree.size));
    // WriteTreeToDotFile(&tree, FILE_OUT);
    // PrintSortedNode(tree.root);
    // CheckSorting(arr_before, tree.root, arr_after, count);

    FILE *file_out = fopen("akinator_out.txt", "w");
    PrintAkinatorToFile(file_out, tree.root);
    fclose(file_out);
    CHECK_ERROR_RETURN(TreeDtor(&tree));
    fclose(Info.file);
    return 0;
}