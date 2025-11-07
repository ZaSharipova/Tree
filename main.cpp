#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "TreeFunctions.h"
#include "Enums.h"
#include "TreeGraph.h"

int compare(const void *a, const void *b) {
    TreeElem_t *res_a = (TreeElem_t *)a;
    TreeElem_t *res_b = (TreeElem_t *)b;
    if (*res_a > *res_b) return 1;
    else if (*res_a < *res_b) return -1;
    else return 0;
}

// int CheckSorting(TreeElem_t *arr_to_check_with, TreeNode_t *head) {
//     assert(arr_to_check_with);
//     assert(head);


// }

int main(void) {

    Tree_t tree = {};
    TreeCtor(&tree);

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


    // int number = 10;
    // InsertTree(&tree, &number);
    // number = 5;
    // InsertTree(&tree, &number);
    // number = 3;
    // InsertTree(&tree, &number);
    // number = 7;
    // InsertTree(&tree, &number);
    // number = 20;
    // InsertTree(&tree, &number);
    // number = 15;
    // InsertTree(&tree, &number);
    // number = 13;
    // InsertTree(&tree, &number);
    // number = 16;
    // InsertTree(&tree, &number);
    // number = 6;
    // InsertTree(&tree, &number);

    WriteTreeToDotFile(&tree, "output.txt");
    PrintSortedNode(tree.root, (arr_after));

    qsort(arr_before, count, sizeof(TreeElem_t), compare);

    for (int i = 0; i < count; i++) {
        if (arr_before[i] != arr_after[i]) {
            printf("AAAAAAA sorting doesn't work.");
            return -1;
        }
    }
    printf("\n");
    for (int i = 0; i < MAX_ARR_SIZE, arr_after[i] != 0; i++) {
        printf("%d ", arr_after[i]);
    }

    TreeDtor(&tree);
    return 0;
}