#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "TreeFunctions.h"
#include "Enums.h"
#include "TreeGraph.h"

int main(void) {
    TreeErrors err = kSuccess;

    Tree_t tree = {};
    CHECK_ERROR_RETURN(TreeRootCtor(&tree));

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

    int number = 0;
    INSERT_TO_TREE(10);
    INSERT_TO_TREE(5);
    INSERT_TO_TREE(3);
    INSERT_TO_TREE(7);
    INSERT_TO_TREE(20);
    INSERT_TO_TREE(15);
    INSERT_TO_TREE(13);
    INSERT_TO_TREE(16);
    INSERT_TO_TREE(6);

    TreeElem_t arr_before[] = {10, 5, 3, 7, 20, 15, 13, 16, 6};
    int count = sizeof(arr_before) / sizeof(arr_before[0]);
    TreeElem_t arr_after[MAX_ARR_SIZE] = {10, 5, 3, 7, 20, 15, 13, 16, 6};

    WriteTreeToDotFile(&tree, FILE_OUT);
    PrintSortedNode(tree.root);

    CheckSorting(arr_before, tree.root, arr_after, count);

    CHECK_ERROR_RETURN(TreeDtor(&tree));
    return 0;
}